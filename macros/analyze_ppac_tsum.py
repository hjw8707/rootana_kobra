#!/usr/bin/env python3
"""
PPAC TSum Fitting 결과 분석 스크립트
각 PPAC의 txsum, tysum이 런 세팅에 따라 얼마나 변하는지 분석
"""

import re
from collections import defaultdict
import statistics

# 런 번호와 이온 종류 매핑
RUN_TO_ION = {
    251: "o19",
    259: "o19",
    304: "o20",
    316: "o21",
    223: "o22",
    410: "ne24",
    420: "ne25",
    425: "ne26"
}

def parse_fit_results(filename):
    """fitting 결과 파일을 파싱"""
    data = defaultdict(lambda: defaultdict(list))  # {ppac: {quant: [(run, mean, sigma), ...]}}

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or not line.startswith('[cpp]'):
                continue

            # [cpp] f1uppac txsum (r251): mean=3894.362, sigma=57.743
            match = re.match(r'\[cpp\]\s+(\w+)\s+(\w+)\s+\(r(\d+)\):\s+mean=([\d.]+),\s+sigma=([\d.]+)', line)
            if match:
                ppac, quant, run_str, mean_str, sigma_str = match.groups()
                run = int(run_str)
                mean = float(mean_str)
                sigma = float(sigma_str)

                data[ppac][quant].append((run, mean, sigma))

    return data

def calculate_statistics(values):
    """통계량 계산"""
    if not values:
        return {}

    return {
        'mean': statistics.mean(values),
        'std': statistics.stdev(values) if len(values) > 1 else 0.0,
        'min': min(values),
        'max': max(values),
        'range': max(values) - min(values),
        'count': len(values)
    }

def format_table(data):
    """결과를 표 형식으로 정리"""
    output = []
    output.append("=" * 100)
    output.append("PPAC TSum Fitting 결과 분석")
    output.append("=" * 100)
    output.append("")

    # PPAC별로 정렬
    ppac_order = ['f1uppac', 'f2dppac', 'f3uppac', 'f3dppac']
    quant_order = ['txsum', 'tysum']

    for ppac in ppac_order:
        if ppac not in data:
            continue

        output.append(f"\n{'=' * 100}")
        output.append(f"PPAC: {ppac.upper()}")
        output.append(f"{'=' * 100}")

        for quant in quant_order:
            if quant not in data[ppac]:
                continue

            results = data[ppac][quant]
            means = [r[1] for r in results]
            sigmas = [r[2] for r in results]

            mean_stats = calculate_statistics(means)
            sigma_stats = calculate_statistics(sigmas)

            output.append(f"\n  {quant.upper()}:")
            output.append(f"    Mean 값 통계:")
            output.append(f"      평균: {mean_stats['mean']:.3f}")
            output.append(f"      표준편차: {mean_stats['std']:.3f}")
            # 최소값과 최대값의 run 번호를 정확히 찾기
            min_run = min((r[0] for r in results if r[1] == mean_stats['min']), default=0)
            max_run = min((r[0] for r in results if r[1] == mean_stats['max']), default=0)
            output.append(f"      최소값: {mean_stats['min']:.3f} (run {min_run})")
            output.append(f"      최대값: {mean_stats['max']:.3f} (run {max_run})")
            output.append(f"      범위: {mean_stats['range']:.3f} ({mean_stats['range']/mean_stats['mean']*100:.2f}%)")
            output.append(f"      데이터 포인트 수: {mean_stats['count']}")

            output.append(f"    Sigma 값 통계:")
            output.append(f"      평균: {sigma_stats['mean']:.3f}")
            output.append(f"      표준편차: {sigma_stats['std']:.3f}")
            output.append(f"      최소값: {sigma_stats['min']:.3f}")
            output.append(f"      최대값: {sigma_stats['max']:.3f}")
            output.append(f"      범위: {sigma_stats['range']:.3f}")

            # 런별 상세 정보
            output.append(f"    런별 상세:")
            for run, mean, sigma in sorted(results):
                ion = RUN_TO_ION.get(run, "unknown")
                output.append(f"      Run {run:3d} ({ion:6s}): mean={mean:8.3f}, sigma={sigma:7.3f}")

    # 요약 테이블
    output.append(f"\n{'=' * 100}")
    output.append("요약: Mean 값의 변화량 및 통계")
    output.append(f"{'=' * 100}")
    output.append(f"{'PPAC':<12} {'Quant':<8} {'변화량':<12} {'최소 Mean':<12} {'최대 Mean':<12} {'평균 Mean':<12} {'표준편차':<12}")
    output.append("-" * 100)

    for ppac in ppac_order:
        if ppac not in data:
            continue
        for quant in quant_order:
            if quant not in data[ppac]:
                continue

            results = data[ppac][quant]
            means = [r[1] for r in results]
            mean_stats = calculate_statistics(means)
            variation_amount = mean_stats['range']  # 변화량 (절대값)

            output.append(f"{ppac:<12} {quant:<8} {variation_amount:>11.3f} {mean_stats['min']:>11.3f} {mean_stats['max']:>11.3f} {mean_stats['mean']:>11.3f} {mean_stats['std']:>11.3f}")

    return "\n".join(output)

def main():
    filename = "macros/ppac_tsum_fit.txt"

    try:
        data = parse_fit_results(filename)

        if not data:
            print("데이터를 찾을 수 없습니다.")
            return

        output = format_table(data)
        print(output)

        # 파일로도 저장
        with open("macros/ppac_tsum_analysis.txt", 'w') as f:
            f.write(output)

        print(f"\n\n결과가 'macros/ppac_tsum_analysis.txt' 파일에 저장되었습니다.")

    except FileNotFoundError:
        print(f"파일을 찾을 수 없습니다: {filename}")
    except Exception as e:
        print(f"오류 발생: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    main()


