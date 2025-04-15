import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import matplotlib.dates as mdates

def merging_excel_files():
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

def re_indexing_control_data():
    df_all = pd.read_pickle('merged_df.pkl')
    df_all.set_index(pd.to_datetime(df_all[df_all.columns[0]]), inplace=True)
    df_all.sort_index(inplace=True)
    df_all.to_pickle('merged_df_indexed.pkl')

def plotting_control_data(start_date, end_date = None, time_cut = False):
    df_all = pd.read_pickle('merged_df_indexed.pkl')

    start_date = pd.to_datetime(start_date)
    if end_date is not None:
        end_date = pd.to_datetime(end_date)

    if time_cut:
        df_all = df_all.between_time('08:00', '20:00')

    df = df_all[df_all[df_all.columns[0]] >= start_date]
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


def plot_multiple_dates(dates, time_cut=False, ncol = 6):
    df_all = pd.read_pickle('merged_df_indexed.pkl')

    if time_cut:
        df_all = df_all.between_time('08:00', '20:00')

    nrows = (len(dates) + ncol - 1) // ncol  # 필요한 행 수 계산
    fig, axs = plt.subplots(nrows, ncol, figsize=(20, 5 * nrows), squeeze=False)

    # 첫 번째 날짜의 데이터로 범례를 설정
    first_date = pd.to_datetime(dates[0])
    df_first = df_all[df_all[df_all.columns[0]].dt.date == first_date.date()]
    columns = [df_first.columns[1], df_first.columns[2], df_first.columns[4], df_first.columns[5]]
    colors = plt.cm.tab10(np.linspace(0, 1, len(columns)))  # 색상 설정

    # 범례를 위한 빈 플롯
    for column, color in zip(columns, colors):
        axs[0, 0].plot([], [], label=column, color=color)

    # 모든 데이터의 y축 범위를 계산
    y_min, y_max = float('inf'), float('-inf')
    for date in dates:
        start_date = pd.to_datetime(date)
        df = df_all[df_all[df_all.columns[0]].dt.date == start_date.date()]
        for column in columns:
            y_min = min(y_min, df[column].min())
            y_max = max(y_max, df[column].max())

    for i, date in enumerate(dates):
        row, col = i // ncol, i % ncol
        start_date = pd.to_datetime(date)
        df = df_all[df_all[df_all.columns[0]].dt.date == start_date.date()]

        for column, color in zip(columns, colors):
            axs[row, col].plot(df[df.columns[0]], df[column], color=color)
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
    plt.show()

if __name__ == '__main__':
    df = pd.read_pickle('merged_df_indexed.pkl')
    dates = pd.to_datetime(df[df.columns[0]].dt.date.unique()).strftime('%Y-%m-%d')
    plot_multiple_dates(dates, True)
    #plotting_control_data('2024-06-19', None, True)