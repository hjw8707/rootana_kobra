#! /usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams['font.family'] = 'AppleGothic'
plt.rcParams['axes.unicode_minus'] = False
import numpy as np
#
# scaler information
#
#  run, ts, ppac, f2pl, f3pl, hwtriga, hwtrigb, refp, reft
#
class Scaler:
    def __init__(self, isotope):
        self.isotope = str(isotope)
        self.all_df = pd.read_csv(f"misc/scaler_ext_{isotope}.csv", index_col=None, skipinitialspace=True)
        self.df = self.all_df
        # ts 컬럼의 단위를 10 ns에서 s로 변환
        self.all_df["ts"] = self.all_df["ts"] / 1e8
        # 시간 관련 컬럼을 제외한 다른 컬럼들의 rate를 계산하여 새로운 컬럼 추가
        self.all_df["ppac_rate"] = self.get_rate_column("ppac")
        self.all_df["f2pl_rate"] = self.get_rate_column("f2pl")
        self.all_df["f3pl_rate"] = self.get_rate_column("f3pl")
        self.all_df["hwtriga_rate"] = self.get_rate_column("hwtriga")
        self.all_df["hwtrigb_rate"] = self.get_rate_column("hwtrigb")
        self.df = self.all_df
        self.aoq = None

    def reset(self):
        self.df = self.all_df

    def select_run(self, run):
        self.df = self.df[self.df["run"] == run]

    def get_rate_column(self, col_name, new_col_name="rate"):
        """
        주어진 컬럼(col_name)의 rate(초당 변화량)를 계산하여 all_df에 새로운 컬럼(new_col_name)으로 추가합니다.
        ts는 이미 초(sec) 단위로 변환되어 있으므로, 단위 변환 없이 rate를 계산하면 됩니다.
        rate = (col[n] - col[n-1]) / (ts[n] - ts[n-1])
        첫 번째 값은 NaN으로 설정됩니다.
        """
        # ts는 이미 초 단위이므로, 그대로 rate 계산
        rate = self.all_df[col_name].diff() / self.all_df["ts"].diff()
        return rate

    def fit_1d(self, xcol, ycol):
        x = self.df[xcol]
        y = self.df[ycol]
        coeffs = np.polyfit(x, y, 1)
        slope = coeffs[0]
        intercept = coeffs[1]
        print(f"slope: {slope}, intercept: {intercept}")
        return slope, intercept

    def draw_plot(self, xcol, ycol, flag_fit = False):
        plt.plot(self.df[xcol], self.df[ycol], label=f"{ycol} vs {xcol}")
        if flag_fit:
            slope, intercept = self.fit_1d(xcol, ycol)
            plt.plot(self.df[xcol], slope * self.df[xcol] + intercept, 'r--', label=u"1차 선형 피팅")
        plt.xlabel(xcol)
        plt.ylabel(ycol)
        plt.legend()
        plt.show()

    def load_aoq(self, file):
        self.aoq = pd.read_csv(file, index_col=None, skipinitialspace=True)

    def draw_ppac_rate(self, flag_aoq = False):
        fig, ax1 = plt.subplots()
        color1 = 'tab:blue'
        ax1.set_xlabel("reft")
        ax1.set_ylabel("ppac_rate", color=color1)
        ax1.plot(self.df["reft"], self.df["ppac_rate"], color=color1, label="ppac_rate")
        ax1.tick_params(axis='y', labelcolor=color1)

        if flag_aoq and self.aoq is not None:
            ax2 = ax1.twinx()
            color2 = 'tab:red'
            ax2.set_ylabel("aoq", color=color2)
            ax2.plot(self.aoq["reft"], self.aoq["aoq"], color=color2, label="aoq")
            ax2.tick_params(axis='y', labelcolor=color2)

        fig.tight_layout()
        plt.show()

    def get_run_total_amount(self, col_name):
        run_total_amount = {}
        for run, group in self.df.groupby("run"):
            run_total_amount[run] = group[col_name].max() - group[col_name].min()
        return run_total_amount

    def get_run_average_rate(self, col_name):
        run_average_rate = {}
        for run, group in self.df.groupby("run"):
            run_average_rate[run] = (group[col_name].max() - group[col_name].min()) / (group["ts"].max() - group["ts"].min())
        return run_average_rate

    def print_run_summary(self):
        run_total_amount = {}
        run_average_rate = {}
        for col in self.df.columns:
            if col not in ["run"]:
                run_total_amount[col] = self.get_run_total_amount(col)
                run_average_rate[col] = self.get_run_average_rate(col)

        print(f"Run Summary for {self.isotope}")
        print("--------------------------------")
        print("run, ts, ppac, ppac_rate, f2pl, f2pl_rate, f3pl, f3pl_rate, hwtriga, hwtriga_rate, hwtrigb, hwtrigb_rate, refp, refp_rate, reft, reft_rate")
        for run, group in self.df.groupby("run"):
            print(f"{run}, {run_total_amount['ts'][run]}, ", end="")
            for col in self.df.columns:
                if col not in ["run", "ts"]:
                    print(f"{run_total_amount[col][run]}, {run_average_rate[col][run]}, ", end="")
            print()

if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1:
        filename = sys.argv[1]
        scaler = Scaler(filename)
    else:
        print("Usage: python scaler.py <filename>")
        sys.exit(1)

    #scaler.select_run(304)
    #scaler.load_aoq("misc/aoq_reft.csv")
    #scaler.draw_ppac_rate(True)
    scaler.print_run_summary()