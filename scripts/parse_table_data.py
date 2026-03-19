#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
표 데이터에서 값과 에러를 분리하여 하나의 표로 생성하는 스크립트
각 Sample별로 값과 에러를 나란히 배치
"""

import re

# 이미지 설명에서 제공된 표 데이터
data = {
    "Spot 1": ["124.8±11.8 µm", "131.1±15.3 µm", "142.6±11.8 µm", "169.11±9.23 µm", "153.8±16.6 µm", "150.46±4.44 µm"],
    "Spot 2": ["97.2±15.2 µm", "177.6±17.0 µm", "166.1±30.1 µm", "157.6±14.1 µm", "165.01±7.35 µm", "140.8±16.0 µm"],
    "Spot 3": ["147.2±13.9 µm", "168.8±12.6 µm", "160.7±14.9 µm", "167.2±11.0 µm", "152.74±9.31 µm", "132.82±6.56 µm"],
    "Spot 4": ["147.5±18.1 µm", "181.6±11.2 µm", "166.0±14.4 µm", "163.7±13.7 µm", "173.7±10.5 µm", "137.42±6.25 µm"],
    "Spot 5": ["163.1±11.0 µm", "188.22±9.96 µm", "172.2±10.4 µm", "158.53±7.26 µm", "160.44±9.93 µm", "148.30±8.38 µm"],
    "Spot 6": ["138.5±18.5 µm", "151.58±9.49 µm", "166.4±15.8 µm", "176.4±17.8 µm", "158.60±8.40 µm", "159.14±5.88 µm"],
    "Spot 7": ["180.0±12.3 µm", "156.81±8.46 µm", "168.46±8.38 µm", "183.1±10.2 µm", "197.0±23.7 µm", "138.69±6.34 µm"],
    "Spot 8": ["201.28±7.59 µm", "155.1±12.1 µm", "176.66±9.97 µm", "190.14±7.95 µm", "184.22±6.81 µm", "133.2±11.1 µm"],
    "Spot 9": ["151.7±10.4 µm", "188.8±13.9 µm", "175.6±20.4 µm", "173.6±14.2 µm", "176.8±13.8 µm", "139.16±8.29 µm"],
    "Spot 10": ["171.4±14.2 µm", "163.1±10.3 µm", "173.8±12.7 µm", "195.2±20.1 µm", "175.8±13.7 µm", "150.6±12.8 µm"],
    "Spot 11": ["177.03±9.12 µm", "142.4±15.6 µm", "166.4±10.3 µm", "182.0±12.7 µm", "181.8±16.3 µm", "116.5±18.1 µm"],
    "Spot 12": ["160.15±6.76 µm", "172.52±9.82 µm", "152.3±14.8 µm", "173.44±9.58 µm", "169.0±10.2 µm", "127.7±32.9 µm"],
    "Spot 13": ["149.2±12.0 µm", "154.27±5.80 µm", "150.4±10.1 µm", "161.10±7.62 µm", "182.8±15.0 µm", "141.21±3.89 µm"],
    "Spot 14": ["161.06±7.49 µm", "140.74±3.92 µm", "164.55±9.88 µm", "181.3±10.3 µm", "175.60±9.92 µm", "138.29±6.53 µm"],
    "Spot 15": ["155.5±15.2 µm", "153.2±19.2 µm", "165.5±10.8 µm", "154.9±24.5 µm", "169.49±7.68 µm", "154.80±5.73 µm"],
    "Spot 16": ["116.7±11.7 µm", "163.70±8.81 µm", "168.0±10.3 µm", "164.04±7.67 µm", "174.3±10.7 µm", "153.01±8.62 µm"],
    "Spot 17": ["", "179.0±10.0 µm", "154.90±6.61 µm", "169.84±2.83 µm", "160.0±15.6 µm", "144.83±8.68 µm"],
    "Spot 18": ["", "177.4±10.4 µm", "147.33±7.61 µm", "188.64±7.69 µm", "153.52±9.13 µm", "147.13±6.72 µm"],
    "Spot 19": ["", "", "134.01±4.34 µm", "168.6±11.2 µm", "150.0±10.3 µm", "136.0±12.9 µm"],
    "Spot 20": ["", "", "136.0±14.2 µm", "163.9±15.0 µm", "158.2±14.7 µm", "140.6±17.0 µm"],
}

def parse_value_error(value_str):
    """값±에러 형식의 문자열을 파싱하여 (값, 에러) 튜플 반환"""
    if not value_str or value_str.strip() == "":
        return (None, None)
    
    # ± 기호를 기준으로 분리
    match = re.match(r'([\d.]+)±([\d.]+)', value_str)
    if match:
        value = float(match.group(1))
        error = float(match.group(2))
        return (value, error)
    return (None, None)

# 값과 에러를 하나의 테이블로 구성 (각 Sample별로 값, 에러 순서로)
combined_table = {}

for spot, samples in data.items():
    combined_table[spot] = []
    
    for sample_value in samples:
        value, error = parse_value_error(sample_value)
        combined_table[spot].append(value)  # 값
        combined_table[spot].append(error)   # 에러

# 표 출력 함수
def print_combined_table():
    print(f"\n{'='*120}")
    print(f"{'값과 에러 통합 표':^120}")
    print(f"{'='*120}")
    
    # 헤더
    header = f"{'Spot':<12}"
    for i in range(1, 7):
        header += f"{'Sample ' + str(i) + ' 값':<15}"
        header += f"{'Sample ' + str(i) + ' 에러':<15}"
    print(header)
    print("-" * 120)
    
    # 데이터 행
    for spot in sorted(combined_table.keys(), key=lambda x: int(x.split()[1])):
        row = f"{spot:<12}"
        for val in combined_table[spot]:
            if val is None:
                row += f"{'':<15}"
            else:
                row += f"{val:<15.2f}"
        print(row)

# 통합 표 출력
print_combined_table()

# CSV 형식으로 저장
def save_combined_to_csv(filename):
    with open(filename, 'w', encoding='utf-8') as f:
        # 헤더
        header = "Spot"
        for i in range(1, 7):
            header += f",Sample {i} 값,Sample {i} 에러"
        f.write(header + "\n")
        
        # 데이터 행
        for spot in sorted(combined_table.keys(), key=lambda x: int(x.split()[1])):
            row = spot
            for val in combined_table[spot]:
                if val is None:
                    row += ","
                else:
                    row += f",{val:.2f}"
            f.write(row + "\n")

save_combined_to_csv("combined_table.csv")

print("\n\nCSV 파일이 생성되었습니다:")
print("  - combined_table.csv: 값과 에러 통합 표")
