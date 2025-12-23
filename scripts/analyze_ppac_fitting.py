#!/usr/bin/env python3
"""
PPAC fitting 결과 분석 스크립트
각 PPAC의 txsum, tysum이 런 세팅에 따라 얼마나 변하는지 분석
"""

import re
from collections import defaultdict

# 제공된 데이터 파싱
data_text = """
[cpp] f1uppac txsum (r251): mean=3894.362, sigma=57.743
[cpp] f1uppac tysum (r251): mean=5815.892, sigma=133.509
[cpp] f2dppac txsum (r251): mean=2933.786, sigma=31.899
[cpp] f2dppac tysum (r251): mean=2856.977, sigma=44.478
[cpp] f3uppac txsum (r251): mean=3239.451, sigma=35.541
[cpp] f3uppac tysum (r251): mean=3226.828, sigma=37.853
[cpp] f3dppac txsum (r251): mean=3656.063, sigma=40.014
[cpp] f3dppac tysum (r251): mean=3700.264, sigma=42.042

[cpp] f1uppac txsum (r259): mean=3897.714, sigma=51.500
[cpp] f1uppac tysum (r259): mean=5814.348, sigma=129.833
[cpp] f2dppac txsum (r259): mean=2933.699, sigma=30.780
[cpp] f2dppac tysum (r259): mean=2855.168, sigma=42.521
[cpp] f3uppac txsum (r259): mean=3245.004, sigma=35.796
[cpp] f3uppac tysum (r259): mean=3233.716, sigma=38.141
[cpp] f3dppac txsum (r259): mean=3662.216, sigma=39.051
[cpp] f3dppac tysum (r259): mean=3708.938, sigma=42.497

[cpp] f1uppac txsum (r304): mean=3872.623, sigma=73.529
[cpp] f1uppac tysum (r304): mean=5798.589, sigma=134.041
[cpp] f2dppac txsum (r304): mean=2933.175, sigma=31.911
[cpp] f2dppac tysum (r304): mean=2856.100, sigma=43.694
[cpp] f3uppac txsum (r304): mean=3242.492, sigma=35.713
[cpp] f3uppac tysum (r304): mean=3233.807, sigma=37.944
[cpp] f3dppac txsum (r304): mean=3657.942, sigma=39.283
[cpp] f3dppac tysum (r304): mean=3706.306, sigma=41.878

[cpp] f1uppac txsum (r316): mean=3889.186, sigma=56.426
[cpp] f1uppac tysum (r316): mean=5803.828, sigma=126.452
[cpp] f2dppac txsum (r316): mean=2933.491, sigma=33.450
[cpp] f2dppac tysum (r316): mean=2860.324, sigma=46.763
[cpp] f3uppac txsum (r316): mean=3244.429, sigma=36.283
[cpp] f3uppac tysum (r316): mean=3236.104, sigma=38.828
[cpp] f3dppac txsum (r316): mean=3657.295, sigma=41.242
[cpp] f3dppac tysum (r316): mean=3706.408, sigma=43.838

[cpp] f1uppac txsum (r223): mean=3897.187, sigma=55.660
[cpp] f1uppac tysum (r223): mean=5823.315, sigma=133.669
[cpp] f2dppac txsum (r223): mean=2937.503, sigma=38.667
[cpp] f2dppac tysum (r223): mean=2874.117, sigma=52.512
[cpp] f3uppac txsum (r223): mean=3240.191, sigma=39.646
[cpp] f3uppac tysum (r223): mean=3228.036, sigma=41.870
[cpp] f3dppac txsum (r223): mean=3651.846, sigma=44.407
[cpp] f3dppac tysum (r223): mean=3698.391, sigma=46.156

[cpp] f1uppac txsum (r410): mean=3871.059, sigma=48.991
[cpp] f1uppac tysum (r410): mean=5759.238, sigma=91.563
[cpp] f2dppac txsum (r410): mean=2915.146, sigma=45.199
[cpp] f2dppac tysum (r410): mean=2837.244, sigma=59.161
[cpp] f3uppac txsum (r410): mean=3207.389, sigma=55.777
[cpp] f3uppac tysum (r410): mean=3209.599, sigma=57.820
[cpp] f3dppac txsum (r410): mean=3587.016, sigma=68.273
[cpp] f3dppac tysum (r410): mean=3627.278, sigma=77.369

[cpp] f1uppac txsum (r420): mean=3843.852, sigma=82.054
[cpp] f1uppac tysum (r420): mean=5768.748, sigma=94.861
[cpp] f2dppac txsum (r420): mean=2938.320, sigma=40.013
[cpp] f2dppac tysum (r420): mean=2865.130, sigma=54.140
[cpp] f3uppac txsum (r420): mean=3233.912, sigma=51.639
[cpp] f3uppac tysum (r420): mean=3228.692, sigma=48.473
[cpp] f3dppac txsum (r420): mean=3614.945, sigma=62.531
[cpp] f3dppac tysum (r420): mean=3663.418, sigma=69.417

[cpp] f1uppac txsum (r425): mean=3866.154, sigma=64.149
[cpp] f1uppac tysum (r425): mean=5774.093, sigma=112.060
[cpp] f2dppac txsum (r425): mean=2947.250, sigma=42.957
[cpp] f2dppac tysum (r425): mean=2882.712, sigma=55.039
[cpp] f3uppac txsum (r425): mean=3234.013, sigma=54.042
[cpp] f3uppac tysum (r425): mean=3228.582, sigma=52.525
[cpp] f3dppac txsum (r425): mean=3621.163, sigma=65.526
[cpp] f3dppac tysum (r425): mean=3659.441, sigma=67.363
"""

# 런 번호와 설정 매핑 (코드에서 추론)
run_settings = {
    251: "o19",
    259: "o19",
    304: "o20",
    316: "o21",
    223: "o22",
    410: "ne24",
    420: "ne25",
    425: "ne26"
}

# 데이터 파싱
pattern = r'\[cpp\] (\w+) (txsum|tysum) \(r(\d+)\): mean=([\d.]+), sigma=([\d.]+)'
matches = re.findall(pattern, data_text)

# 데이터프레임 생성
data = []
for ppac, quant, run, mean, sigma in matches:
    data.append({
        'run': int(run),
        'setting': run_settings.get(int(run), 'unknown'),
        'ppac': ppac,
        'quant': quant,
        'mean': float(mean),
        'sigma': float(sigma)
    })

df = pd.DataFrame(data)

print("=" * 80)
print("PPAC Fitting 결과 분석")
print("=" * 80)
print()

# 1. 전체 데이터 요약 테이블
print("1. 전체 데이터 요약")
print("-" * 80)
summary_table = df.pivot_table(
    index=['run', 'setting', 'ppac'],
    columns='quant',
    values=['mean', 'sigma'],
    aggfunc='first'
)
print(summary_table.to_string())
print()

# 2. 각 PPAC별 mean 값의 변화 분석
print("\n2. 각 PPAC별 Mean 값 변화 분석")
print("-" * 80)

for ppac in df['ppac'].unique():
    print(f"\n[{ppac}]")
    ppac_data = df[df['ppac'] == ppac]

    for quant in ['txsum', 'tysum']:
        quant_data = ppac_data[ppac_data['quant'] == quant].sort_values('run')
        means = quant_data['mean'].values
        runs = quant_data['run'].values
        settings = quant_data['setting'].values

        print(f"\n  {quant}:")
        print(f"    Run별 값:")
        for run, setting, mean, sigma in zip(runs, settings, means, quant_data['sigma'].values):
            print(f"      r{run:3d} ({setting:5s}): mean={mean:8.3f}, sigma={sigma:6.3f}")

        # 통계량 계산
        mean_avg = np.mean(means)
        mean_std = np.std(means)
        mean_min = np.min(means)
        mean_max = np.max(means)
        mean_range = mean_max - mean_min
        mean_cv = (mean_std / mean_avg) * 100  # 변동계수 (%)

        print(f"    통계:")
        print(f"      평균: {mean_avg:8.3f}")
        print(f"      표준편차: {mean_std:6.3f}")
        print(f"      범위 (min-max): {mean_min:8.3f} ~ {mean_max:8.3f} (범위: {mean_range:6.3f})")
        print(f"      변동계수: {mean_cv:5.2f}%")

# 3. 설정별 그룹 분석
print("\n\n3. 설정(Setting)별 그룹 분석")
print("-" * 80)

for setting in sorted(df['setting'].unique()):
    setting_data = df[df['setting'] == setting]
    runs = sorted(setting_data['run'].unique())
    print(f"\n[{setting}] - Runs: {runs}")

    for ppac in sorted(setting_data['ppac'].unique()):
        ppac_setting = setting_data[setting_data['ppac'] == ppac]
        print(f"  {ppac}:")
        for quant in ['txsum', 'tysum']:
            quant_data = ppac_setting[ppac_setting['quant'] == quant]
            if len(quant_data) > 0:
                mean_val = quant_data['mean'].mean()
                std_val = quant_data['mean'].std() if len(quant_data) > 1 else 0
                print(f"    {quant}: {mean_val:8.3f} ± {std_val:6.3f}")

# 4. 설정 간 비교 (같은 PPAC, 같은 quant)
print("\n\n4. 설정 간 비교 (변화량 분석)")
print("-" * 80)

for ppac in sorted(df['ppac'].unique()):
    print(f"\n[{ppac}]")
    for quant in ['txsum', 'tysum']:
        quant_data = df[(df['ppac'] == ppac) & (df['quant'] == quant)].sort_values('setting')

        # 설정별 평균값 계산
        setting_means = quant_data.groupby('setting')['mean'].mean()

        print(f"\n  {quant}:")
        print(f"    설정별 평균:")
        for setting, mean_val in setting_means.items():
            print(f"      {setting:5s}: {mean_val:8.3f}")

        # 전체 범위와 변동
        overall_mean = quant_data['mean'].mean()
        overall_std = quant_data['mean'].std()
        overall_range = quant_data['mean'].max() - quant_data['mean'].min()

        print(f"    전체 통계:")
        print(f"      전체 평균: {overall_mean:8.3f}")
        print(f"      표준편차: {overall_std:6.3f}")
        print(f"      전체 범위: {overall_range:6.3f}")
        print(f"      변동률: {(overall_range/overall_mean)*100:5.2f}%")

print("\n" + "=" * 80)
print("분석 완료")
print("=" * 80)
