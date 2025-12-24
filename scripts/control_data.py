import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import matplotlib.dates as mdates

class ControlData:
    def __init__(self):
        if os.path.exists('misc/merged_df_indexed.pkl'):
            self.df = pd.read_pickle('misc/merged_df_indexed.pkl')
        else:
            print('merged_df_indexed.pkl does not exist')
            print('Please run merging_excel_files() and re_indexing_control_data() first')

    def merging_excel_files(self):
        # 디렉토리의 모든 셀 파일 찾기
        excel_files = glob.glob(os.path.join('../misc/2024_KoBRA_Control_Data', '*.xlsx'))

        # 모든 셀 파일을 하나의 데이터프레임으로 병합하고 첫 번째 열을 날짜-시간으로 정렬
        dataframes = []
        for file in excel_files:
            df = pd.read_excel(file)
            df = df.drop(0)
            dataframes.append(df)
            print(f'{file} is loaded')

        merged_df = pd.concat(dataframes)
        print(merged_df.columns)
        print(merged_df.head())
        # 첫 번째 열을 날짜-시간 형식으로 변환
        merged_df[merged_df.columns[0]] = pd.to_datetime(merged_df[merged_df.columns[0]])

        merged_df.sort_values(by=merged_df.columns[0], inplace=True)
        merged_df.to_pickle('merged_df.pkl')

    def re_indexing_control_data(self):
        df = pd.read_pickle('merged_df.pkl')
        df.set_index(pd.to_datetime(df[df.columns[0]]), inplace=True)
        df.sort_index(inplace=True)
        df.to_pickle('merged_df_indexed.pkl')

    def plotting_control_data_day(self, start_date, end_date = None, time_cut = False):
        start_date = pd.to_datetime(start_date)
        if end_date is not None:
            end_date = pd.to_datetime(end_date)

        if time_cut:
            df = self.df.between_time('08:00', '20:00')
        else:
            df = self.df

        df = df[df[df.columns[0]] >= start_date]
        if end_date is not None:
            df = df[df[df.columns[0]] <= end_date + pd.Timedelta(days=1)]
        else:
            df = df[df[df.columns[0]].dt.date == start_date.date()]

        columns = [df.columns[1], df.columns[2], df.columns[4], df.columns[5]]
        plt.figure()
        for column in columns:
            plt.plot(df[df.columns[0]], df[column], label=column)
        plt.xlabel(df.columns[0])
        plt.ylabel('Values')
        plt.title(f'Multiple Curves vs {df.columns[0]}')
        plt.legend()
        plt.grid(True)
        plt.show()

    def plotting_control_data_time(self, date, start_time, end_time, columns = [1, 2], flucData = None):
        if not isinstance(date, pd.Timestamp):
            date = pd.to_datetime(date)
        df = self.df[self.df[self.df.columns[0]].dt.date == date.date()]
        df = df.between_time(start_time, end_time)
        columns = [df.columns[col] for col in columns]
        colors = ['red', 'orange', 'yellow', 'green', 'blue', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan', 'magenta']
        plt.figure()
        ax1 = plt.gca()
        lns = []
        for column, color in zip(columns, colors):
            lns.append(ax1.plot(df[df.columns[0]], df[column], label=column, color=color)[0])
        if flucData is not None:
            df_fluc = pd.read_csv(flucData)
            print(df_fluc.head())
            df_fluc[df_fluc.columns[0]] = pd.to_datetime(date.date()) + pd.to_timedelta(df_fluc[df_fluc.columns[0]], unit='s')
            ax1.set_ylabel('Magnet B-field [T]')
            ax2 = ax1.twinx()
            lns.append(ax2.plot(df_fluc[df_fluc.columns[0]], df_fluc[df_fluc.columns[1]], label='Fluctuation Data', color='blue')[0])
            ax2.set_ylabel('Measured A/Q')

        plt.xlabel(df.columns[0])
        plt.title(f'Fluctuation Data of {date.date()}')

        # for legends
        labs = [l.get_label() for l in lns]
        ax1.legend(lns, labs, loc='lower left')
        plt.grid(True)
        plt.show()

    def analysis_control_data_time(self, date, start_time, end_time, columns = [1, 2], flucData = None):
        if not isinstance(date, pd.Timestamp):
            date = pd.to_datetime(date)
        df = self.df[self.df[self.df.columns[0]].dt.date == date.date()]
        df = df.between_time(start_time, end_time)
        columns = [df.columns[col] for col in columns]
        for column in columns:
            mean = df[column].mean()
            std = df[column].std()
            max_val = df[column].max()
            min_val = df[column].min()
            print(f"{column}의 평균: {mean}")
            print(f"{column}의 표준편차: {std}")
            print(f"{column}의 최대값: {max_val}")
            print(f"{column}의 최소값: {min_val}")
            # 표준편차, 최대값, 최소값을 평균에 대한 퍼센트로도 출력
            if mean != 0:
                std_percent = (std / mean) * 100
                max_percent = (max_val / mean - 1) * 100
                min_percent = (min_val / mean - 1) * 100
                print(f"{column}의 표준편차(평균 대비 %): {std_percent:.2f}%")
                print(f"{column}의 최대값(평균 대비 %): {max_percent:.2f}%")
                print(f"{column}의 최소값(평균 대비 %): {min_percent:.2f}%")
            else:
                print(f"{column}의 평균이 0이어서 퍼센트 계산 불가")
            print("-" * 30)
        # flucData에 대해서도 같은 분석 진행
        if flucData is not None:
            print("----- Fluctuation Data(aoq) 분석 -----")
            # flucData 파일 읽기
            df_fluc = pd.read_csv(flucData)
            # 첫 번째 컬럼이 시간(초)라고 가정, 두 번째 컬럼이 aoq
            aoq_col = df_fluc.columns[1]
            mean = df_fluc[aoq_col].mean()
            std = df_fluc[aoq_col].std()
            max_val = df_fluc[aoq_col].max()
            min_val = df_fluc[aoq_col].min()
            print(f"{aoq_col}의 평균: {mean}")
            print(f"{aoq_col}의 표준편차: {std}")
            print(f"{aoq_col}의 최대값: {max_val}")
            print(f"{aoq_col}의 최소값: {min_val}")
            if mean != 0:
                std_percent = (std / mean) * 100
                max_percent = (max_val / mean - 1) * 100
                min_percent = (min_val / mean - 1) * 100
                print(f"{aoq_col}의 표준편차(평균 대비 %): {std_percent:.2f}%")
                print(f"{aoq_col}의 최대값(평균 대비 %): {max_percent:.2f}%")
                print(f"{aoq_col}의 최소값(평균 대비 %): {min_percent:.2f}%")
            else:
                print(f"{aoq_col}의 평균이 0이어서 퍼센트 계산 불가")
            print("-" * 30)


    def plot_multiple_dates(self, dates, cols = [1, 2, 4, 5], save_name = None, time_cut=False, show_plot = True, ncol = 6):
        df_base = self.df

        if time_cut:
            df_base = df_base.between_time('08:00', '20:00')

        nrows = (len(dates) + ncol - 1) // ncol  # 필요한 행 수 계산
        # 전체 그림의 사이즈를 픽셀 단위로 지정 (예: 2000x1000 픽셀)
        dpi = 100  # 해상도 설정 (예: 100dpi)
        width_px = 2000
        height_px = 200 * nrows  # 각 행마다 300픽셀 할당 (필요에 따라 조정)
        figsize = (width_px / dpi, height_px / dpi)
        fig, axs = plt.subplots(nrows, ncol, figsize=figsize, dpi=dpi, squeeze=False)

        # 첫 번째 날짜의 데이터로 범례를 설정
        first_date = pd.to_datetime(dates[0])
        df_first = df_base[df_base[df_base.columns[0]].dt.date == first_date.date()]
        columns = [df_first.columns[col] for col in cols]
        colors = plt.cm.tab10(np.linspace(0, 1, len(columns)))  # 색상 설정

        # 범례를 위한 빈 플롯
        for column, color in zip(columns, colors):
            axs[0, 0].plot([], [], label=column, color=color)

        # 모든 데이터의 y축 범위를 계산
        y_min, y_max = float('inf'), float('-inf')
        for date in dates:
            start_date = pd.to_datetime(date)
            df_day = df_base[df_base[df_base.columns[0]].dt.date == start_date.date()]
            if df_day.empty:
                continue
            for column in columns:
                y_min = min(y_min, df_day[column].min())
                y_max = max(y_max, df_day[column].max())

        # 모든 날짜에서 데이터가 하나도 없으면(또는 전부 NaN이면) 기본 범위로 폴백
        if not np.isfinite(y_min) or not np.isfinite(y_max):
            y_min, y_max = 0.0, 1.0

        for i, date in enumerate(dates):
            row, col = i // ncol, i % ncol
            start_date = pd.to_datetime(date)
            df_day = df_base[df_base[df_base.columns[0]].dt.date == start_date.date()]

            for column, color in zip(columns, colors):
                axs[row, col].plot(df_day[df_day.columns[0]], df_day[column], color=color)
            axs[row, col].set_ylabel('Values')
            axs[row, col].set_title(f'{start_date.date()}')
            axs[row, col].grid(True)

            # 시간 범위 설정
            start_time = start_date.replace(hour=8, minute=0)
            end_time = start_date.replace(hour=20, minute=0)
            axs[row, col].set_xlim([start_time, end_time])

            # 모든 플롯에 동일한 y축 범위 설정
            axs[row, col].set_ylim([y_min, y_max])

        # 사용하지 않는 서브플롯 숨기기
        for i in range(len(dates), nrows * ncol):
            row, col = i // ncol, i % ncol
            axs[row, col].set_visible(False)

        # 전체 플롯에 대한 범례 추가
        fig.legend(loc='lower right')

        fig.autofmt_xdate()
        plt.tight_layout()
        if save_name is not None: plt.savefig(save_name)
        if show_plot: plt.show()

if __name__ == '__main__':
    cd = ControlData()
    dates = pd.to_datetime(cd.df[cd.df.columns[0]].dt.date.unique()).strftime('%Y-%m-%d')
    # 파일로 저장하려면, plot_multiple_dates 함수 내에서 plt.savefig('파일이름.png')를 plt.show() 전에 추가하면 됩니다.
    # 예시:
    # plot_multiple_dates(dates, cols = [1, 2, 4, 5, 7, 8], time_cut=True)
    # plt.savefig('multiple_dates_plot.png')
    # plt.show()
    #plot_multiple_dates(dates, cols = [1, 2, 4, 5, 7, 8], time_cut=True, save_name = 'multiple_dates_plot.png')
    #plot_multiple_dates(dates, cols = [1, 2], time_cut=True, save_name = 'D1_plot.png', show_plot = False)
    #plot_multiple_dates(dates, cols = [4, 5], time_cut=True, save_name = 'D2_plot.png', show_plot = False)
    #plot_multiple_dates(dates, cols = [7, 8], time_cut=True, save_name = 'Swinger_plot.png', show_plot = False)
    #cd.analysis_control_data_time('2024-07-18', '14:40', '15:50', columns = [1, 2], flucData = 'misc/aoq_reft.csv')
    #cd.plotting_control_data_time('2024-07-18', '14:40', '15:50', columns = [1, 2], flucData = 'misc/aoq_reft.csv')
    #print(cd.df.columns)
    #cd.plot_multiple_dates(dates = dates, cols = [9], time_cut=True, save_name = 'Dump_plot.png', show_plot = True)
    cd.plot_multiple_dates(dates = dates, cols = [10], time_cut=True, save_name = 'Collimator_plot.png', show_plot = True)