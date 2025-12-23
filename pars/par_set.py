#!/usr/bin/env python3
#                else:

# par_set.py
#
# 파라미터 일괄 변경 스크립트
#
# 일단은 PPAC의 timing cut에 대해서만 처리
import argparse
import os

ppac_list = [ "pars/PAR_F1UPPACX.txt",
              "pars/PAR_F2DPPAC.txt",
              "pars/PAR_F3UPPAC.txt",
              "pars/PAR_F3DPPAC.txt",
              "pars/PAR_F2PPTD.txt",
              "pars/PAR_F3PPTU.txt",
              "pars/PAR_F1UPPAC.txt"]

tdc_cut = {
    "no": [(0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000)],
    "phys_pid": [(42000, 53000), (51000, 57000), (51500, 70000), (52000, 68000), (40000, 57000), (50000, 65000), (42000, 53000)],
    "phys_mdis": [(35000, 50000), (30000, 55000), (50000, 58000), (50000, 60000), (30000, 55000), (50000, 56000), (35000, 50000)],
}

txsum_cut = {
    "no": [(0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000)],
    "yes": [(2800, 4200), (2700, 3100), (2000, 4000), (3500, 3800), (2800, 4200), (2800, 4200), (2800, 4200)],
    "loose": [(0, 5000), (0, 3500), (400, 4000), (800, 5000), (0, 3500), (800, 5000), (0, 5000)],
    "phys_pid": [(3566, 4201), (2761, 3109), (3055, 3430), (3444, 3867), (0, 500000), (0, 500000), (3566, 4201)],
    "phys_mdis": [(3602, 4113), (2751, 3112), (2999, 3460), (3367, 3893), (0, 500000), (0, 500000), (3602, 4113)],
}

tysum_cut = {
    "no": [(0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000), (0, 500000)],
    "yes": [(0, 500000), (2500, 3100), (2000, 4000), (3500, 3900), (2800, 4200), (2800, 4200), (2800, 4200)],
    "loose": [(0, 5000), (400, 4800), (400, 4000), (1000, 5000), (400, 4000), (1000, 5000), (0, 7000)],
    "phys_pid": [(5155, 6447), (2620, 3104), (3035, 3432), (3482, 3924), (0, 500000), (0, 500000), (5155, 6447)],
    "phys_mdis": [(5149, 6429), (2601, 3091), (2993, 3451), (3407, 3948), (0, 500000), (0, 500000), (5149, 6429)],
}

def set_ppac_timing_cut(fileindex, timing = "no", txsum = "no", tysum = "no"):
    # 파일을 읽어서 줄 단위로 리스트에 저장
    with open(ppac_list[fileindex], 'r') as f:
        lines = f.readlines()
    if timing in tdc_cut:
        lines[3] = f"{tdc_cut[timing][fileindex][0]} {tdc_cut[timing][fileindex][1]}\n"
    with open(ppac_list[fileindex], 'w') as f:
        if "PPT" not in ppac_list[fileindex]:
            if txsum in txsum_cut and tysum in tysum_cut:
                lines[4] = f"{txsum_cut[txsum][fileindex][0]} {txsum_cut[txsum][fileindex][1]} {tysum_cut[tysum][fileindex][0]} {tysum_cut[tysum][fileindex][1]}\n"
        f.writelines(lines)
    print(f"Set PPAC timing cut to {timing}, txsum cut to {txsum}, tysum cut to {tysum} for {ppac_list[fileindex]}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--timing_cut", type=str, help="Timing cut" , choices=["no","phys_pid", "phys_mdis"], default="no")
    parser.add_argument("-x", "--txsum_cut", type=str, help="TXSum cut" , choices=["no", "yes", "loose", "phys_pid", "phys_mdis"], default="no")
    parser.add_argument("-y", "--tysum_cut", type=str, help="TYSum cut" , choices=["no", "yes", "loose", "phys_pid", "phys_mdis"], default="no")
    args = parser.parse_args()
    for fileindex in range(len(ppac_list)):
        set_ppac_timing_cut(fileindex, args.timing_cut, args.txsum_cut, args.tysum_cut)
