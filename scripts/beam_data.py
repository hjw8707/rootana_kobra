import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import matplotlib.dates as mdates
import math
from datetime import datetime
from runtime_data import RunTimeData

class BeamData:
    def __init__(self):
        if os.path.exists('misc/beam_data_merged.pkl'):
            self.df = pd.read_pickle('misc/beam_data_merged.pkl')
        else:
            print('beam_data_merged.pkl does not exist')
            print('Please run merging_dataframes() first')

    def get_date_list(self):
        directory = 'misc/Beam'
        return [name for name in os.listdir(directory) if os.path.isdir(os.path.join(directory, name))]

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
        주어진 기간 내에서 nan이 아닌 sort 컬럼 값들의 평균을 반환하는 함수.
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
        return filtered[sort].mean()


    def get_weighted_mean_by_time(self, sort, date_start, time_start, time_end, date_end=None):
        """
        빔 세기(sort)의 시간에 대한 가중 평균을 계산하는 함수입니다.
        시간 간격(초 단위)을 가중치로 하여, 각 구간의 빔 세기 평균에 대해 가중 평균을 구합니다.

        Args:
            sort (str): 빔 세기 칼럼명
            date_start (str): 시작 날짜 ('YYYY-MM-DD')
            time_start (str): 시작 시간 ('HH:MM:SS')
            time_end (str): 끝 시간 ('HH:MM:SS')
            date_end (str, optional): 끝 날짜 ('YYYY-MM-DD'), None이면 date_start와 동일

        Returns:
            float: 시간 가중 평균값 (값이 없으면 None)
        """
        filtered = self.get_data_list(sort, date_start, time_start, time_end, date_end)
        if filtered is None or filtered.empty:
            return None

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
            return None
        return weighted_sum / total_weight



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

    def plot_data_lists_all_days(self, sorts, y_min=None, y_max=None):
        sort = sorts

        date_list = self.get_date_list()
        n_dates = len(date_list)
        ncols = 6
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
        mean_val = self.get_weighted_mean_by_time(sort, start_date_str, start_str, stop_str, stop_date_str)
        if mean_val is None:
            print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 평균값이 없습니다.")
            return None, None
        integral_val = mean_val * (stop_time - start_time).total_seconds()
        print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 평균값: {mean_val}")
        print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 {sort} 적분값: {integral_val}")
        return mean_val, integral_val


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
            mean_val = self.get_weighted_mean_by_time('LEBT', start_date_str, start_str, stop_str, stop_date_str)
            if mean_val is None:
                print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 LEBT 평균값이 없습니다.")
                continue
            integral_val = mean_val * (stop_time - start_time).total_seconds()
            print(f"Run {run_number} ({start_date_str} {start_str} ~ {stop_date_str} {stop_str})의 LEBT 평균, 적분값: {mean_val}, {integral_val}")
            df_result = pd.concat([df_result, pd.DataFrame({'run_number': [run_number], 'start_time': [start_time], 'stop_time': [stop_time], 'LEBT_mean': [mean_val], 'LEBT_integral': [integral_val]})], ignore_index=True)
        df_result.to_csv('misc/beam_time.csv', index=False)
        return df_result

if __name__ == '__main__':
    beam_data = BeamData()
    # print(beam_data.df.head())
    # print(beam_data.df.columns)
    # print(beam_data.get_data('2024-07-02', '10:00:00', 'FC1_1K'))
    # print(beam_data.get_data_list('FC1_1M', '2024-07-07', '09:00:00', '09:10:00'))
    # print(beam_data.get_data_mean('FC1_1M', '2024-07-07', '09:00:00', '09:10:00'))
    # beam_data.plot_data_list('FC1_1M', '2024-07-07', '09:00:00', '09:10:00')
    #beam_data.plot_data_lists(['FC1_1M', 'FC2_1M', 'LEBT', 'P2DT'], '2024-07-08', '08:00:00', '19:00:00')
    #beam_data.plot_data_lists_all_days(['FC1_1M', 'FC2_1M'], y_min=-5e-7, y_max=2e-6)
    #beam_data.plot_fc_all_days()
    #beam_data.plot_beam_all_days()
    # df = beam_data.merging_dataframes()
    # print(df.head())
    # print(df.columns)
    #print(beam_data.get_data_mean('LEBT', '2024-07-10', '10:00:00', '11:00:00'))
    #print(beam_data.get_weighted_mean_by_time('LEBT', '2024-07-10', '10:00:00', '11:00:00'))
    #print(beam_data.get_beam_average_by_run(166, 'LEBT '))
    beam_data.get_beam_average_csv()