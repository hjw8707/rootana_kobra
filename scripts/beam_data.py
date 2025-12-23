import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import matplotlib.dates as mdates
import math
from datetime import datetime, timedelta
from runtime_data import RunTimeData

class BeamData:
    def __init__(self):
        if os.path.exists('misc/beam_data_merged.pkl'):
            self.df = pd.read_pickle('misc/beam_data_merged.pkl')
        else:
            print('beam_data_merged.pkl does not exist')
            print('Please run merging_dataframes() first')

    def get_date_list(self, flagRunOnly=False):
        directory = 'misc/Beam'
        date_list = [name for name in os.listdir(directory) if os.path.isdir(os.path.join(directory, name))]
        if flagRunOnly:
            date_list = [d for d in date_list if d > '0716']
        return date_list

    def get_dataframe(self, date, sort): # sort can be 'FC1_1K, FC1_1M, FC2_1K, FC2_1M, LEBT, P2DT'
        directory = f'misc/Beam/{date}'
        filename = {
            'FC1_1K': 'KoBRA-DIAG_FC001_Current_1K.csv',
            'FC1_1M': 'KoBRA-DIAG_FC001_Current_1M.csv',
            'FC2_1K': 'KoBRA-DIAG_FC002_Current_1K.csv',
            'FC2_1M': 'KoBRA-DIAG_FC002_Current_1M.csv',
            'LEBT': 'LEBT-DIAG_ACCT001_Current.csv',
            'P2DT': 'P2DT-DIAG_ACCT001_Current.csv'
        }
        df = pd.read_csv(f'{directory}/{filename[sort]}')
        # 첫 번째 컬럼을 date-time 형식으로 변환
        df[df.columns[0]] = pd.to_datetime(df[df.columns[0]])
        return df

    def merging_dataframes(self):
        merged_dfs = []
        for date in self.get_date_list():
            print(date)
            # 각 종류별 데이터프레임 불러오기
            try:
                df_fc1_1k = self.get_dataframe(date, 'FC1_1K')
                df_fc1_1m = self.get_dataframe(date, 'FC1_1M')
                df_fc2_1k = self.get_dataframe(date, 'FC2_1K')
                df_fc2_1m = self.get_dataframe(date, 'FC2_1M')
                df_lebt = self.get_dataframe(date, 'LEBT')
                df_p2dt = self.get_dataframe(date, 'P2DT')
            except Exception as e:
                print(f"{date} 데이터 로딩 중 오류 발생: {e}")
                continue

            # 첫 번째 컬럼명을 datetime으로 통일
            time_col = df_fc1_1k.columns[0]
            # merge를 위해 컬럼명 통일
            df_fc1_1k = df_fc1_1k.rename(columns={time_col: 'datetime', df_fc1_1k.columns[1]: 'FC1_1K'})
            df_fc1_1m = df_fc1_1m.rename(columns={time_col: 'datetime', df_fc1_1m.columns[1]: 'FC1_1M'})
            df_fc2_1k = df_fc2_1k.rename(columns={df_fc2_1k.columns[0]: 'datetime', df_fc2_1k.columns[1]: 'FC2_1K'})
            df_fc2_1m = df_fc2_1m.rename(columns={df_fc2_1m.columns[0]: 'datetime', df_fc2_1m.columns[1]: 'FC2_1M'})
            df_lebt = df_lebt.rename(columns={df_lebt.columns[0]: 'datetime', df_lebt.columns[1]: 'LEBT'})
            df_p2dt = df_p2dt.rename(columns={df_p2dt.columns[0]: 'datetime', df_p2dt.columns[1]: 'P2DT'})

            # datetime 기준으로 merge (inner join, 필요시 how='outer'로 변경)
            df_merged = df_fc1_1k.merge(df_fc2_1k, on='datetime', how='outer')
            df_merged = df_merged.merge(df_fc1_1m, on='datetime', how='outer')
            df_merged = df_merged.merge(df_fc2_1m, on='datetime', how='outer')
            df_merged = df_merged.merge(df_lebt, on='datetime', how='outer')
            df_merged = df_merged.merge(df_p2dt, on='datetime', how='outer')
            merged_dfs.append(df_merged)

        # 모든 날짜 데이터프레임을 하나로 합치기
        if merged_dfs:
            total_merged_df = pd.concat(merged_dfs, ignore_index=True)
            total_merged_df.sort_values(by='datetime', inplace=True)
            # timezone 정보 삭제
            total_merged_df['datetime'] = total_merged_df['datetime'].dt.tz_localize(None)
            # 완벽하게 겹치는 row 삭제
            total_merged_df.drop_duplicates(inplace=True)
            total_merged_df.reset_index(drop=True, inplace=True)
            total_merged_df.to_pickle('misc/beam_data_merged.pkl')
            print("병합된 데이터가 'beam_data_merged.pkl'로 저장되었습니다.")
            return total_merged_df
        else:
            print("병합할 데이터가 없습니다.")
            return None

    def get_data(self, sort, date, time):
        """
        self.df에서 주어진 date, time에 가장 가까운 sort 값을 가지는 NaN이 아닌 칼럼의 값을 반환하는 함수

        Args:
            date (str): 'YYYY-MM-DD' 형식의 날짜
            time (str): 'HH:MM:SS' 형식의 시간
            sort (str): 찾고자 하는 칼럼명

        Returns:
            해당 row의 sort 칼럼 값 (없으면 None)
        """
        if not hasattr(self, 'df') or self.df is None or self.df.empty:
            print("self.df가 비어있거나 존재하지 않습니다.")
            return None

        # datetime 컬럼이 datetime 타입이 아닐 경우 변환
        if not pd.api.types.is_datetime64_any_dtype(self.df['datetime']):
            self.df['datetime'] = pd.to_datetime(self.df['datetime'])

        # 입력받은 date, time을 datetime으로 변환
        try:
            target_dt = pd.to_datetime(f"{date} {time}")
        except Exception as e:
            print(f"날짜/시간 변환 오류: {e}")
            return None

        # sort 칼럼이 존재하는지 확인
        if sort not in self.df.columns:
            print(f"'{sort}' 칼럼이 존재하지 않습니다.")
            return None

        # NaN이 아닌 sort 값만 필터링
        valid_df = self.df[self.df[sort].notna()]
        if valid_df.empty:
            print(f"'{sort}' 칼럼에 NaN이 아닌 값이 없습니다.")
            return None

        # 가장 가까운 datetime 찾기 (NaN이 아닌 값 중에서)
        time_diffs = (valid_df['datetime'] - target_dt).abs()
        min_idx = time_diffs.idxmin()

        return valid_df.loc[min_idx, sort]

    def get_data_list(self, sort, date_start, time_start, time_end, date_end=None):
        if not hasattr(self, 'df') or self.df is None or self.df.empty:
            print("self.df가 비어있거나 존재하지 않습니다.")
            return None

        if not pd.api.types.is_datetime64_any_dtype(self.df['datetime']):
            self.df['datetime'] = pd.to_datetime(self.df['datetime'])

        if date_end is None:
            date_end = date_start

        try:
            start_dt = pd.to_datetime(f"{date_start} {time_start}")
            end_dt = pd.to_datetime(f"{date_end} {time_end}")
        except Exception as e:
            print(f"날짜/시간 변환 오류: {e}")
            return None

        if sort not in self.df.columns:
            print(f"'{sort}' 칼럼이 존재하지 않습니다.")
            return None

        # 기간 내 nan이 아닌 값만 필터링
        mask = (self.df['datetime'] >= start_dt) & (self.df['datetime'] <= end_dt) & (self.df[sort].notna())
        filtered = self.df.loc[mask, ['datetime', sort]]

        if filtered.empty:
            print(f"지정한 기간 내 '{sort}' 칼럼에 nan이 아닌 값이 없습니다.")
            return None

        return filtered

    def get_data_mean(self, sort, date_start, time_start, time_end, date_end=None):
        """
        주어진 기간 내에서 nan이 아닌 sort 컬럼 값들의 평균과 표준편차를 반환하는 함수.
        date_end가 None이면 date_start와 같은 날로 간주.

        Args:
            sort (str): 평균을 구할 칼럼명
            date_start (str): 시작 날짜 ('YYYY-MM-DD')
            time_start (str): 시작 시간 ('HH:MM:SS')
            time_end (str): 끝 시간 ('HH:MM:SS')
            date_end (str, optional): 끝 날짜 ('YYYY-MM-DD'), None이면 date_start와 동일

        Returns:
            float: 평균값 (값이 없으면 None)
        """
        filtered = self.get_data_list(sort, date_start, time_start, time_end, date_end)
        return filtered[sort].mean(), filtered[sort].std()


    def get_weighted_mean_by_time(self, sort, date_start, time_start, time_end, date_end=None):
        """
        빔 세기(sort)의 시간에 대한 가중 평균과 표준 편차를 계산하는 함수입니다.
        시간 간격(초 단위)을 가중치로 하여, 각 구간의 빔 세기 평균에 대해 가중 평균을 구합니다.

        Args:
            sort (str): 빔 세기 칼럼명
            date_start (str): 시작 날짜 ('YYYY-MM-DD')
            time_start (str): 시작 시간 ('HH:MM:SS')
            time_end (str): 끝 시간 ('HH:MM:SS')
            date_end (str, optional): 끝 날짜 ('YYYY-MM-DD'), None이면 date_start와 동일

        Returns:
            float: 시간 가중 평균값 (값이 없으면 None)
            float: 시간 가중 표준 편차값 (값이 없으면 None)
        """
        filtered = self.get_data_list(sort, date_start, time_start, time_end, date_end)
        if filtered is None or filtered.empty:
            return None, None

        # datetime 기준으로 정렬
        filtered = filtered.sort_values('datetime').reset_index(drop=True)

        # 시간 간격(초) 계산
        times = filtered['datetime'].values
        values = filtered[sort].values

        if len(times) < 2:
            # 데이터가 1개뿐이면 해당 값 반환
            return values[0] if len(values) == 1 else None

        # 각 구간의 시간 간격(초) 계산
        time_diffs = (filtered['datetime'].shift(-1) - filtered['datetime']).dt.total_seconds()[:-1]
        # 마지막 구간은 전체 구간의 평균 간격으로 보정
        if len(time_diffs) > 0:
            avg_diff = time_diffs.mean()
        else:
            avg_diff = 0
        time_diffs = time_diffs.tolist()
        time_diffs.append(avg_diff)

        # 가중 평균 계산
        weighted_sum = 0
        total_weight = 0
        for v, w in zip(values, time_diffs):
            if pd.notna(v) and w > 0:
                weighted_sum += v * w
                total_weight += w

        if total_weight == 0:
            return None, None
        return weighted_sum / total_weight, np.sqrt(np.sum(time_diffs * (values - weighted_sum / total_weight)**2) / total_weight**2)



    def plot_data_list(self, sort, date_start, time_start, time_end, date_end=None):
        filtered = self.get_data_list(sort, date_start, time_start, time_end, date_end)
        if filtered is None or filtered.empty:
            return None
        plt.plot(filtered['datetime'], filtered[sort])
        return plt.gca()


    def plot_data_lists(self, sorts, date_start, time_start, time_end, date_end=None, ax=None):
        for sort in sorts:
            filtered = self.get_data_list(sort, date_start, time_start, time_end, date_end)
            if filtered is None or filtered.empty:
                continue
            if ax is None:
                plt.plot(filtered['datetime'], filtered[sort])
            else:
                ax.plot(filtered['datetime'], filtered[sort])
        if ax is None:
            plt.show()
        return plt.gca()

    def plot_data_lists_all_days(self, sorts, y_min=None, y_max=None, date_list=None):
        sort = sorts

        if date_list is None:
            date_list = self.get_date_list()
        n_dates = len(date_list)
        ncols = 3
        nrows = math.ceil(n_dates / ncols)

        fig, axes = plt.subplots(nrows=nrows, ncols=ncols, figsize=(16, 3 * nrows), sharex=False)
        axes = axes.flatten() if n_dates > 1 else [axes]

        for idx, date in enumerate(date_list):
            # date가 mmdd 형식이므로, 연도를 현재 연도로 지정하여 pandas datetime으로 변환
            year = '2024'
            date_pd = pd.to_datetime(f"{year}{date}", format="%Y-%m-%d")
            ax = axes[idx]
            self.plot_data_lists(sort, date_pd, '08:00:00', '19:00:00', ax=ax)
            ax.set_title(f"{date_pd.strftime('%Y-%m-%d')}")
            # x축 tick 값 표시 줄이기 (예: 4개만 표시) + 시간만 표시
            ax.xaxis.set_major_locator(plt.MaxNLocator(8))
            ax.xaxis.set_major_formatter(plt.matplotlib.dates.DateFormatter('%H'))
            if y_min is not None and y_max is not None:
                ax.set_ylim(y_min, y_max)
            #ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: f"{x:.2g}"))

        # 남은 subplot은 숨김 처리
        for j in range(idx + 1, len(axes)):
            axes[j].axis('off')

        # 플롯 사이의 간격을 더 줄이기 위해 tight_layout 대신 subplots_adjust 사용
        plt.subplots_adjust(wspace=0.55, hspace=0.45)  # 간격을 더 줄임 (필요시 더 조정)
        # 바깥쪽 마진(figure의 여백)을 줄이기 위해 subplots_adjust의 left, right, top, bottom 값을 조정합니다.
        plt.subplots_adjust(left=0.04, right=0.98, top=0.93, bottom=0.07, wspace=0.3, hspace=0.45)

        # 오른쪽 아래에 legend 표시
        handles = []
        labels = []
        for i, s in enumerate(sort):
            handles.append(plt.Line2D([], [], color=plt.cm.tab10(i), label=s))
            labels.append(s)
        fig.legend(handles=handles, labels=labels, loc='lower right')
        plt.show()
        return fig

    def plot_fc_all_days(self):
        fig = self.plot_data_lists_all_days(['FC1_1M', 'FC2_1M'], y_min=-5e-7, y_max=2e-6)
        fig.savefig('misc/FC_plot.pdf', bbox_inches='tight')
        return fig

    def plot_beam_all_days(self):
        fig = self.plot_data_lists_all_days(['LEBT', 'P2DT'], y_min=-3e-6, y_max=5e-5)
        fig.savefig('misc/beam_plot.pdf', bbox_inches='tight')
        return fig

    def get_beam_average_by_run(self, run_number, sort, runtime_csv_path='misc/midcheck_summary.csv'):
        """
        RunTimeData 클래스를 이용하여 특정 run의 시작/끝 시간을 얻고,
        해당 구간의 beam 평균값을 계산합니다.

        :param run_number: run 번호 (int 또는 str)
        :param sort: 'LEBT', 'P2DT', 'FC1_1M' 등
        :param runtime_csv_path: RunTimeData에서 사용할 CSV 경로
        :return: 평균값 (float)
        """
        # RunTimeData 인스턴스 생성
        runtime_data = RunTimeData(runtime_csv_path)
        try:
            start_time = runtime_data.get_start_time(run_number)
            stop_time = runtime_data.get_stop_time(run_number)
        except Exception as e:
            print(f"런타임 정보 조회 실패: {e}")
            return None

        # 시작/끝 시간이 numpy.datetime64일 수 있으므로 pandas.Timestamp로 변환
        start_time = pd.to_datetime(start_time)
        stop_time = pd.to_datetime(stop_time)

        # 날짜 추출 (YYYY-MM-DD)
        start_date_str = start_time.strftime('%Y-%m-%d')
        stop_date_str = stop_time.strftime('%Y-%m-%d')
        # 시간 추출 (HH:MM:SS)
        start_str = start_time.strftime('%H:%M:%S')
        stop_str = stop_time.strftime('%H:%M:%S')

        # 평균값 계산
        mean_val, std_val = self.get_weighted_mean_by_time(sort, start_date_str, start_str, stop_str, stop_date_str)
        if mean_val is None:
            print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 평균값이 없습니다.")
            return None, None, None, None
        integral_val = mean_val * (stop_time - start_time).total_seconds()
        print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 평균값: {mean_val} ± {std_val}")
        print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 적분값: {integral_val} ± {std_val * (stop_time - start_time).total_seconds()}")
        return mean_val, std_val, integral_val, (stop_time - start_time).total_seconds()


    def get_beam_average_csv(self, runtime_csv_path='misc/midcheck_summary.csv'):
        """
        RunTimeData 클래스를 이용하여 특정 run의 시작/끝 시간을 얻고,
        해당 구간의 beam 평균값을 계산합니다.

        :param run_number: run 번호 (int 또는 str)
        :param sort: 'LEBT', 'P2DT', 'FC1_1M' 등
        :param runtime_csv_path: RunTimeData에서 사용할 CSV 경로
        :return: 평균값 (float)
        """
        # RunTimeData 인스턴스 생성
        runtime_data = RunTimeData(runtime_csv_path)
        df = runtime_data.get_dataframe()
        df_result = pd.DataFrame(columns=['run_number', 'start_time', 'stop_time', 'LEBT_mean', 'LEBT_integral'])
        for index, row in df.iterrows():
            run_number = row['run_number']
            start_time = row['start_time']
            stop_time = row['stop_time']
            start_date_str = start_time.strftime('%Y-%m-%d')
            stop_date_str = stop_time.strftime('%Y-%m-%d')
            start_str = start_time.strftime('%H:%M:%S')
            stop_str = stop_time.strftime('%H:%M:%S')
            mean_val, std_val = self.get_weighted_mean_by_time('LEBT', start_date_str, start_str, stop_str, stop_date_str)
            if mean_val is None:
                print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 LEBT 평균값이 없습니다.")
                continue
            integral_val = mean_val * (stop_time - start_time).total_seconds()
            print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 LEBT 평균, 적분값: {mean_val} ± {std_val}, {integral_val} ± {std_val * (stop_time - start_time).total_seconds()}")
            df_result = pd.concat([df_result, pd.DataFrame({'run_number': [run_number], 'start_time': [start_time], 'stop_time': [stop_time], 'LEBT_mean': [mean_val], 'LEBT_std': [std_val], 'LEBT_integral': [integral_val], 'LEBR_integral_std': [std_val * (stop_time - start_time).total_seconds()]})], ignore_index=True)
        df_result.to_csv('misc/beam_time.csv', index=False)
        return df_result

    def get_beam_ratio(self, date_start, time_start, time_end, date_end=None):
        lebt_mean, lebt_std = self.get_weighted_mean_by_time('LEBT', date_start, time_start, time_end, date_end)
        p2dt_mean, p2dt_std = self.get_weighted_mean_by_time('P2DT', date_start, time_start, time_end, date_end)
        if lebt_mean is None or p2dt_mean is None:
            return None
        return p2dt_mean/lebt_mean

    def get_beam_mean_ratio(self):
        ratio_list = []
        ratio_list.append(self.get_beam_ratio('2024-07-15', '13:32:20', '13:35:00'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '13:52:00', '13:59:50'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '14:18:50', '14:20:20'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '14:57:00', '15:06:00'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '15:22:00', '15:29:30'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '16:06:15', '16:06:55'))
        ratio_list.append(self.get_beam_ratio('2024-07-15', '16:36:30', '16:43:00'))
        mean_ratio = np.mean(ratio_list)
        std_ratio = np.std(ratio_list)
        return mean_ratio, std_ratio

    def get_groups_above_threshold(self, sort, threshold, date_start, time_start, time_end, date_end = None, upper_limit = 5.5e-7):
        df = self.get_data_list(sort, date_start, time_start, time_end, date_end)
        if df is None or df.empty:
            return None
        # threshold를 넘는 연속 구간별로 그룹화하여 데이터프레임 리스트로 반환
        mask = df[sort] > threshold
        mask = mask & (df[sort] < upper_limit)
        groups = []
        current_group = []
        for idx, above in enumerate(mask):
            if above:
                current_group.append(df.iloc[idx])
            else:
                if current_group:
                    groups.append(pd.DataFrame(current_group))
                    current_group = []
        if current_group:
            groups.append(pd.DataFrame(current_group))

        # INSERT_YOUR_CODE
        # 그룹 간의 시간 차이가 3초 이내면 하나의 그룹으로 합치기
        if len(groups) <= 1:
            return groups

        merged_groups = []
        current_group = groups[0].copy()
        for i in range(1, len(groups)):
            prev_group = current_group
            next_group = groups[i]
            prev_end_time = prev_group.iloc[-1]['datetime']
            next_start_time = next_group.iloc[0]['datetime']
            # 시간 차이가 3초 이내면 병합
            if (next_start_time - prev_end_time).total_seconds() <= 3:
                # 두 그룹을 합쳐서 새로운 current_group으로
                current_group = pd.concat([prev_group, next_group], ignore_index=True)
            else:
                merged_groups.append(current_group)
                current_group = next_group.copy()
        merged_groups.append(current_group)
        groups = merged_groups

        # INSERT_YOUR_CODE
        # 요소가 하나인 그룹은 제거
        groups = [g for g in groups if len(g) > 1]

        return groups

    def get_timing_nsamples_mean_std(self, df, sort):
        if df is None or df.empty or sort not in df.columns:
            return None
        return df.iloc[0]['datetime'], len(df), df[sort].mean(), df[sort].std()

    def get_group_properties(self, groups, sort):
        properties = []
        for group in groups:
            timing, nsamples, mean, std = self.get_timing_nsamples_mean_std(group, sort)
            property = {
                'timing': timing,
                'nsamples': nsamples,
                'mean': mean,
                'std': std
            }
            properties.append(property)
        return properties

    def match_properties_by_timing(self, prop1, prop2, threshold_sec=3, flagWithNoMatch=False, flagPrint=False):
        matched_pairs = []
        used_prop2_idx = set()

        for k_idx, k_prop in enumerate(prop1):
            k_time = k_prop['timing']
            found = False
            for m_idx, m_prop in enumerate(prop2):
                if m_idx in used_prop2_idx:
                    continue
                m_time = m_prop['timing']
                # 둘 다 pandas.Timestamp라고 가정
                if abs((k_time - m_time).total_seconds()) <= threshold_sec:
                    matched_pairs.append((k_prop, m_prop))
                    used_prop2_idx.add(m_idx)
                    found = True
                    break
            if not found:
                matched_pairs.append((k_prop, None))

        # fc1_1m_properties 중에서 매칭되지 않은 것도 출력
        for m_idx, m_prop in enumerate(prop2):
            if m_idx not in used_prop2_idx:
                matched_pairs.append((None, m_prop))

        matched_pairs_filtered = matched_pairs if flagWithNoMatch else [pair for pair in matched_pairs if pair[0] is not None and pair[1] is not None]

        if flagPrint:
            print(f"=== timing이 {threshold_sec}초 이내로 매칭된 prop1 - prop2 그룹 ===")
            for pair in matched_pairs_filtered:
                k_prop, m_prop = pair
                k_str = (f"{k_prop['timing'].strftime('%Y-%m-%d %H:%M:%S')} ({k_prop['nsamples']:3d}개, {k_prop['mean']:.2e} ± {k_prop['std']:.2e})") if k_prop else "None"
                m_str = (f"{m_prop['timing'].strftime('%H:%M:%S')} ({m_prop['nsamples']:3d}개, {m_prop['mean']:.2e} ± {m_prop['std']:.2e})") if m_prop else "None"
                ratio = k_prop['mean'] / m_prop['mean']
                ratio_err = ratio * ((k_prop['std']/k_prop['mean'])**2 + (m_prop['std']/m_prop['mean'])**2) ** 0.5
                print(f"prop1: {k_str}  <==>  prop2: {m_str}, ratio = {ratio:.3f} ± {ratio_err:.3f}")

        return matched_pairs_filtered

    def compare_1k_1m(self, sort, start_date, end_date = None):
        groups_1k = self.get_groups_above_threshold(sort + '_1K', 1.0e-7, start_date, '08:00:00', '19:00:00', end_date)
        groups_1m = self.get_groups_above_threshold(sort + '_1M', 2.0e-7, start_date, '08:00:00', '19:00:00', end_date)
        properties_1k = self.get_group_properties(groups_1k, sort + '_1K')
        properties_1m = self.get_group_properties(groups_1m, sort + '_1M')
        matched_pairs = self.match_properties_by_timing(properties_1k, properties_1m, 3, flagPrint=True)
        # INSERT_YOUR_CODE
        # ratio의 sample 수에 대한 가중 평균과 에러 계산
        ratios = []
        ratio_errs = []
        weights = []
        for pair in matched_pairs:
            k_prop, m_prop = pair
            if k_prop is not None and m_prop is not None:
                ratio = k_prop['mean'] / m_prop['mean']
                ratio_err = ratio * ((k_prop['std']/k_prop['mean'])**2 + (m_prop['std']/m_prop['mean'])**2) ** 0.5
                n_samples = min(k_prop['nsamples'], m_prop['nsamples'])
                ratios.append(ratio)
                ratio_errs.append(ratio_err)
                weights.append(n_samples)
        if len(ratios) == 0:
            print("매칭된 데이터가 없습니다.")
            return None, None
        ratios = np.array(ratios)
        ratio_errs = np.array(ratio_errs)
        weights = np.array(weights)
        # 가중 평균
        weighted_mean = np.average(ratios, weights=weights)
        # 가중 표준 오차 계산 (샘플 수에 대한 가중치)
        weighted_var = np.average((ratios - weighted_mean)**2, weights=weights)
        weighted_std = np.sqrt(weighted_var / len(ratios))
        # 개별 에러도 고려한 가중 평균의 에러 (propagate)
        weighted_err = np.sqrt(np.sum((weights * ratio_errs)**2)) / np.sum(weights)
        print(f"샘플수 가중 평균: {weighted_mean:.4f} ± {weighted_err:.4f} (propagate), ± {weighted_std:.4f} (표본분산)")
        return weighted_mean, weighted_err
        #self.plot_data_lists([sort + '_1K', sort + '_1M'], start_date, '08:00:00', '19:00:00', end_date)

if __name__ == '__main__':
    beam_data = BeamData()
    #print(beam_data.df.head())
    #print(beam_data.df.columns)
    # print(beam_data.get_data('2024-07-02', '10:00:00', 'FC1_1K'))
    # print(beam_data.get_data_list('FC1_1M', '2024-07-07', '09:00:00', '09:10:00'))
    # print(beam_data.get_data_mean('FC1_1M', '2024-07-07', '09:00:00', '09:10:00'))
    #beam_data.plot_data_list('FC1_1M', '2024-07-07', '09:00:00', '09:10:00')
    beam_data.plot_data_lists(['LEBT'], '2024-08-13', '13:05:00', '13:50:00')
    #beam_data.plot_data_lists_all_days(['FC1_1K', 'FC1_1M', 'FC2_1K', 'FC2_1M'], y_min=-5e-7, y_max=2e-6)
    #beam_data.plot_fc_all_days()
    #beam_data.plot_beam_all_days()
    # df = beam_data.merging_dataframes()
    # print(df.head())
    # print(df.columns)
    #print(beam_data.get_data_mean('LEBT', '2024-07-10', '10:00:00', '11:00:00'))
    #print(beam_data.get_weighted_mean_by_time('LEBT', '2024-07-10', '10:00:00', '11:00:00'))
    #print(beam_data.get_beam_average_by_run(166, 'LEBT '))
    # beam_data.get_beam_average_csv()
    #print(beam_data.get_data('LEBT', '2024-07-15', '14:00:00'))
    #print(beam_data.get_data('P2DT', '2024-07-15', '14:00:00'))
    #beam_data.plot_data_lists(['LEBT', 'P2DT'], '2024-07-15', '12:00:00', '17:00:00')
    #mean_ratio, std_ratio = beam_data.get_beam_mean_ratio()
    #print(f'beam_mean_ratio: {mean_ratio*100:.2f}% ± {std_ratio*100:.2f}%')



    # 7/16(2024-07-16) 이전 날짜는 제거하여 출력
    #date_list = beam_data.get_date_list()
    #filtered_date_list = [d for d in date_list if d > '0716']
    #beam_data.plot_data_lists_all_days(['FC1_1K', 'FC1_1M'], y_min=-5e-7, y_max=2e-6, date_list=filtered_date_list)
    #beam_data.plot_data_lists_all_days(['FC2_1K', 'FC2_1M'], y_min=-5e-7, y_max=2e-6, date_list=filtered_date_list)
    #beam_data.plot_data_lists(['FC1_1K', 'FC1_1M'], '2024-07-17', '08:00:00', '19:00:00', date_end='2024-08-22')
    #beam_data.compare_1k_1m('FC1', '2024-07-17', '2024-08-23')
    #beam_data.compare_1k_1m('FC2', '2024-07-17', '2024-08-23')