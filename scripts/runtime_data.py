import pandas as pd

class RunTimeData:
    def __init__(self, csv_path):
        self.csv_path = csv_path
        self.df = pd.read_csv(
            csv_path,
            dtype={'run_number': str, 'elapsed_sec': float}
        )
        # run_number를 int로 변환 (앞자리 0 유지 필요시 str로 두세요)
        self.df['run_number'] = self.df['run_number'].astype(int)
        # 날짜-시간 컬럼 변환
        self.df['start_time'] = pd.to_datetime(self.df['start_time'])
        self.df['stop_time'] = pd.to_datetime(self.df['stop_time'])
        # elapsed_sec는 이미 float

    def get_dataframe(self):
        """DataFrame 전체 반환"""
        return self.df

    def get_run(self, run_number):
        return self.df[self.df['run_number'] == int(run_number)]

    def get_start_time(self, run_number):
        return self.df[self.df['run_number'] == int(run_number)]['start_time'].values[0]

    def get_stop_time(self, run_number):
        return self.df[self.df['run_number'] == int(run_number)]['stop_time'].values[0]

    def get_elapsed_stats(self):
        """경과 시간(elapsed_sec) 통계 정보 반환"""
        return self.df['elapsed_sec'].describe()

    def filter_by_elapsed(self, min_sec=None, max_sec=None):
        elapsed = self.df['elapsed_sec']
        cond = pd.Series([True] * len(self.df))
        if min_sec is not None:
            cond &= (elapsed >= min_sec)
        if max_sec is not None:
            cond &= (elapsed <= max_sec)
        return self.df[cond]

    def to_csv(self, out_path):
        """현재 DataFrame을 새로운 CSV로 저장"""
        self.df.to_csv(out_path, index=False)


if __name__ == "__main__":
    runtime_data = RunTimeData('scripts/midcheck_summary.csv')
    df = runtime_data.get_dataframe()
    print(df.head())

    print(runtime_data.get_run(1))
    print(runtime_data.get_start_time(1))
    print(runtime_data.get_stop_time(1))
    #    print(runtime_data.get_elapsed_stats())
