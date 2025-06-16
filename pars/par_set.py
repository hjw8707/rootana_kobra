#!/usr/bin/env python3
#
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
              "pars/PAR_F3PPTU.txt"]

def set_ppac_timing_cut(filename, low = 0, high = 500000):
    # 파일을 읽어서 줄 단위로 리스트에 저장
    with open(filename, 'r') as f:
        lines = f.readlines()
    # 네 번째 줄(인덱스 3)에 low와 high를 공백으로 구분하여 저장
    if len(lines) < 4:
        # 파일에 줄이 4개 미만이면 빈 줄을 추가
        while len(lines) < 4:
            lines.append('\n')
    lines[3] = f"{low} {high}\n"
    # 파일을 다시 씀
    with open(filename, 'w') as f:
        f.writelines(lines)
    print(f"Set PPAC timing cut to {low} {high} for {filename}")

def set_all_ppac_no_timing_cut():
    for filename in ppac_list:
        set_ppac_timing_cut(filename)

def set_all_ppac_timing_cut(set = "phys_pid"):
    if set == "phys_pid":
        set_ppac_timing_cut(ppac_list[0], 43000, 53000)
        set_ppac_timing_cut(ppac_list[1], 50000, 57000)
        set_ppac_timing_cut(ppac_list[2], 50000, 70000)
        set_ppac_timing_cut(ppac_list[3], 50000, 70000)
        set_ppac_timing_cut(ppac_list[4], 53000, 56000)
        set_ppac_timing_cut(ppac_list[5], 57000, 62000)
    elif set == "phys_mdis":
        set_ppac_timing_cut(ppac_list[0], 35000, 50000)
        set_ppac_timing_cut(ppac_list[1], 44000, 54000)
        set_ppac_timing_cut(ppac_list[2], 52000, 58000)
        set_ppac_timing_cut(ppac_list[3], 53000, 58000)
        set_ppac_timing_cut(ppac_list[4], 40000, 52000)
        set_ppac_timing_cut(ppac_list[5], 51500, 55000)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--no_timing_cut", action="store_true")
    parser.add_argument("-t", "--timing_cut", type=str)
    args = parser.parse_args()
    if args.no_timing_cut:
        set_all_ppac_no_timing_cut()
    elif args.timing_cut:
        set_all_ppac_timing_cut(args.timing_cut)