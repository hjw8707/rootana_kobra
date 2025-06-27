import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import glob
import matplotlib.dates as mdates

class BeamData:
    def __init__(self):
        if os.path.exists('scripts/beam_data_merged.pkl'):
            self.df = pd.read_pickle('scripts/beam_data_merged.pkl')
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
            total_merged_df.reset_index(drop=True, inplace=True)
            total_merged_df.to_pickle('scripts/beam_data_merged.pkl')
            print("병합된 데이터가 'beam_data_merged.pkl'로 저장되었습니다.")
        else:
            print("병합할 데이터가 없습니다.")


if __name__ == '__main__':
    beam_data = BeamData()
    print(beam_data.df.head())
    print(beam_data.df.columns)