#!/usr/bin/env python3
"""
CSV 파일에서 Date, Time, FC2 컬럼만 추출하여 데이터프레임으로 만드는 스크립트
LEBT 컬럼도 추가 (BeamData 클래스 사용)
"""

import pandas as pd
import os
from datetime import datetime, timedelta
from beam_data import BeamData

def extract_fc2_data(csv_path, add_lebt=False, beam_data=None, range_sec=30):
    """
    CSV 파일에서 Date, Time, FC2 컬럼만 추출

    Parameters:
    -----------
    csv_path : str
        CSV 파일 경로
    add_lebt : bool, optional
        True이면 LEBT 컬럼도 추가 (기본값: False)
    beam_data : BeamData, optional
        BeamData 인스턴스. add_lebt=True일 때 필요. None이면 새로 생성

    Returns:
    --------
    pd.DataFrame
        Date, Time, FC2 컬럼을 포함한 데이터프레임 (add_lebt=True이면 LEBT도 포함)
    """
    # CSV 파일 읽기
    df = pd.read_csv(csv_path)

    # Date, Time, FC2 컬럼만 선택
    # FC2 컬럼명이 "FC2 [enA]"로 되어 있음
    columns_to_extract = ['Date', 'Time', 'FC2 [enA]']

    # 필요한 컬럼만 추출
    df_extracted = df[columns_to_extract].copy()

    # FC2 컬럼명을 간단하게 변경 (선택사항)
    df_extracted.rename(columns={'FC2 [enA]': 'FC2'}, inplace=True)

    # FC2 값이 "-"인 경우 NaN으로 변환
    df_extracted['FC2'] = df_extracted['FC2'].replace('-', pd.NA)

    # FC2를 숫자형으로 변환 (NaN은 그대로 유지)
    df_extracted['FC2'] = pd.to_numeric(df_extracted['FC2'], errors='coerce')

    # LEBT 컬럼 추가 (요청된 경우)
    if add_lebt:
        df_extracted = add_lebt_column(df_extracted, beam_data, range_sec)

    return df_extracted


def add_lebt_column(df, beam_data=None, range_sec=30):
    """
    데이터프레임에 LEBT 컬럼 추가
    각 행의 Date와 Time에 대해 ±range_sec초 구간의 가중 평균을 계산

    Parameters:
    -----------
    df : pd.DataFrame
        Date, Time 컬럼을 포함한 데이터프레임
    beam_data : BeamData, optional
        BeamData 인스턴스. None이면 새로 생성

    Returns:
    --------
    pd.DataFrame
        LEBT 컬럼이 추가된 데이터프레임
    """
    if beam_data is None:
        beam_data = BeamData()

    # LEBT 컬럼 초기화
    lebt_values = []

    for idx, row in df.iterrows():
        date_str = str(row['Date'])
        time_str = str(row['Time'])

        # Date 형식 변환: "07/12" -> "2024-07-12"
        try:
            month, day = date_str.split('/')
            date_formatted = f"2024-{month.zfill(2)}-{day.zfill(2)}"
        except:
            lebt_values.append(None)
            continue

        # Time 형식 변환: "16:18" -> "16:18:00"
        try:
            hour, minute = time_str.split(':')
            time_base = f"{hour.zfill(2)}:{minute.zfill(2)}:00"

            # ±30초 구간 계산
            time_base_dt = datetime.strptime(time_base, '%H:%M:%S')
            time_start_dt = time_base_dt - timedelta(seconds=range_sec)
            time_end_dt = time_base_dt + timedelta(seconds=range_sec)

            time_start = time_start_dt.strftime('%H:%M:%S')
            time_end = time_end_dt.strftime('%H:%M:%S')
        except:
            lebt_values.append(None)
            continue

        # LEBT 평균값 계산
        try:
            lebt_mean, lebt_std = beam_data.get_weighted_mean_by_time(
                'LEBT',
                date_formatted,
                time_start,
                time_end
            )
            lebt_values.append(lebt_mean)
        except Exception as e:
            print(f"행 {idx} (Date: {date_str}, Time: {time_str})에서 LEBT 계산 오류: {e}")
            lebt_values.append(None)

    # LEBT 컬럼 추가
    df_with_lebt = df.copy()
    df_with_lebt['LEBT'] = lebt_values

    return df_with_lebt


def add_ratio_column(df):
    """
    데이터프레임에 FC2/LEBT ratio 컬럼 추가

    Parameters:
    -----------
    df : pd.DataFrame
        FC2와 LEBT 컬럼을 포함한 데이터프레임

    Returns:
    --------
    pd.DataFrame
        Ratio 컬럼이 추가된 데이터프레임
    """
    df_with_ratio = df.copy()

    # FC2/LEBT ratio 계산 (둘 다 유효한 값일 때만)
    df_with_ratio['Ratio'] = df_with_ratio.apply(
        lambda row: row['FC2'] / row['LEBT']
        if pd.notna(row['FC2']) and pd.notna(row['LEBT']) and row['LEBT'] != 0
        else None,
        axis=1
    )

    return df_with_ratio


def format_scientific_notation(value, precision=4):
    """
    값을 유효숫자 네자리의 지수형식으로 포맷팅

    Parameters:
    -----------
    value : float or None
        포맷팅할 값
    precision : int
        유효숫자 자릿수 (기본값: 4)

    Returns:
    --------
    str
        지수형식으로 포맷팅된 문자열 (예: "1.234e+05")
    """
    if pd.isna(value) or value is None:
        return ''

    # 지수형식으로 변환 (유효숫자 네자리)
    # .{precision-1}e 형식은 정수부 1자리 + 소수부 (precision-1)자리 = 총 precision자리
    return f"{value:.{precision-1}e}"


def save_to_csv_with_formatting(df, output_path):
    """
    데이터프레임을 CSV로 저장하되, LEBT와 Ratio 컬럼은 지수형식으로 포맷팅

    Parameters:
    -----------
    df : pd.DataFrame
        저장할 데이터프레임
    output_path : str
        출력 CSV 파일 경로
    """
    df_output = df.copy()

    # LEBT와 Ratio 컬럼을 지수형식으로 포맷팅
    if 'LEBT' in df_output.columns:
        df_output['LEBT'] = df_output['LEBT'].apply(format_scientific_notation)

    if 'Ratio' in df_output.columns:
        df_output['Ratio'] = df_output['Ratio'].apply(format_scientific_notation)

    # CSV로 저장
    df_output.to_csv(output_path, index=False)
    print(f"\nCSV 파일이 저장되었습니다: {output_path}")


if __name__ == "__main__":
    # 스크립트 위치 기준으로 상대 경로 설정
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)
    csv_path = os.path.join(project_root, 'misc', 'fc002_july.csv')

    # 데이터 추출 (LEBT 포함)
    print("데이터 추출 중...")
    beam_data = BeamData()
    df = extract_fc2_data(csv_path, add_lebt=True, beam_data=beam_data, range_sec=10)

    # Ratio 컬럼 추가
    print("Ratio 컬럼 추가 중...")
    df = add_ratio_column(df)

    # 결과 출력
    print("\n추출된 데이터프레임 (Date, Time, FC2, LEBT, Ratio):")
    print(df)
    print("\n데이터프레임 정보:")
    print(df.info())
    print("\n기본 통계:")
    print(df.describe())

    # 처음 10개 행 출력
    print("\n처음 10개 행:")
    print(df.head(10))

    # LEBT가 있는 행만 필터링해서 보여주기
    df_lebt_valid = df[df['LEBT'].notna()]
    print(f"\nLEBT 값이 있는 행 수: {len(df_lebt_valid)} / {len(df)}")

    # Ratio가 있는 행만 필터링해서 보여주기
    df_ratio_valid = df[df['Ratio'].notna()]
    print(f"Ratio 값이 있는 행 수: {len(df_ratio_valid)} / {len(df)}")

    # CSV로 저장 (LEBT와 Ratio는 지수형식으로 포맷팅)
    output_csv_path = os.path.join(project_root, 'misc', 'faraday_cup_fc2_lebt.csv')
    save_to_csv_with_formatting(df, output_csv_path)

    # LEBT가 2.0e-5 이상, FC2가 400 이상인 것만 필터링해서 Ratio 평균 구하기
    df_lebt_positive = df[(df['LEBT'] > 2.0e-5) & (df['FC2'] > 400)]
    print(f"\nLEBT가 2.0e-5 이상, FC2가 400 이상인 행 수: {len(df_lebt_positive)} / {len(df)}")

    print("\nLEBT가 2.0e-5 이상, FC2가 400 이상인 데이터:")
    print(df_lebt_positive.to_string())
    ratio_mean = df_lebt_positive['Ratio'].mean()
    ratio_std = df_lebt_positive['Ratio'].std()
    print(f"Ratio 평균 (LEBT > 2.0e-5, FC2 > 400): {ratio_mean:.4e}")
    print(f"Ratio 표준편차 (LEBT > 2.0e-5, FC2 > 400): {ratio_std:.4e}, ratio [%]: {(ratio_std/ratio_mean)*100:.2f}")