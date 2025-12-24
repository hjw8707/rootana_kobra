# KO-24-30 빔 세기 산출 방법론

## 목적

이 문서는 KO-24-30 분석 로그/코드베이스에서 사용한 **Beam intensity(빔 세기)** 산출 방법을 “재현 가능”하게 정리한 것입니다.
핵심은 아래 두 레이어를 구분해서 이해하는 것입니다.

- **레벨 A (분석 코드에서 실제로 쓰는 값)**: run 구간 동안의 **LEBT 전류(A)**를 시간가중 평균으로 구한 `LEBT_mean`(A) 및 적분한 `LEBT_integral`(A·s)
- **레벨 B (물리 단위로의 확장)**: 필요할 경우, 위 전류를 **pps(입자/초)** 또는 **flux(단위면적당 rate)**로 변환하는 절차(펄스 duty/전하상태/전달계수/감쇠 등)

---

## Notion 참조: `Beam flux` 페이지(레벨 B의 “정의/변환 사슬” 출처)

- **페이지**: `https://www.notion.so/214dfd46b21d80058303fc7e4b999cf7`
- **이 문서에서의 역할**
  - 레벨 A(코드 구현)는 레포에서 재현/검증 가능하지만, “왜 pps로 변환할 때 저 항들이 들어가는지”는 실험/계측 정의가 필요합니다.
  - 그 정의/변환 사슬은 노션 `Beam flux` 페이지에 정리되어 있으므로, **LEBT→pps 변환을 할 때는 해당 페이지를 공식 출처로 참조**합니다.

### Notion에 기록된 핵심 아이디어(요약)

- **LEBT peak → average 전류 변환**: 펄스 빔인 경우
  - $ I_{avg} = I_{peak} \times \mathrm{PulseWidth}\,[\mathrm{s}] \times \mathrm{Repetition}\,[\mathrm{Hz}] $
- **전하상태 보정**: $Q$ (예: Ar\(^{8+}\) → $Q=8$)
- **전류(A) → pps 변환**:
  - $ \text{pps} = I_{avg} / (Qe) $ (전자 전하 $e \approx 1.60\times 10^{-19}$ )
- **계측/전송/감쇠 보정**:
  - 1M↔1K scale 변환
  - attenuation factor
  - LEBT→P2DT, P2DT→F0 전송 계수
  - F0 acceptance(F0 beam image 기반)

### Notion에 기록된 계수(값 그대로 발췌)

> 아래 값들은 노션 `Beam flux` 페이지에 “예시/메모”로 기록된 수치이며,
> 실제 분석에 적용할 때는 사용한 데이터 구간/조건에 맞게 재검증이 필요합니다.

- **1M-to-1k conversion factor**: $0.8241 \pm 0.0135$
- **LEBT → P2DT conversion factor**: $0.8880 \pm 0.0049$
- **P2DT → F0 conversion factor**: `0.839? (should be checked)` (노션에 “체크 필요”로 표기)
- **LEBT 단위 관련 메모**: “이제보니 측정된 LEBT 값은 [A]가 맞는 것 같다. Attenuation이 고려되지 않은 것도 확인할 것.”

### Notion에 기록된 “Total F0 beam intensity” 수식(구조 유지)

```math
I_{\mathrm{pps}} =
\frac{
  \mathrm{LEBT}_{\mathrm{peak}} \times \mathrm{PulseWidth} \times \mathrm{Repetition}
}{
  Qe
}
\times (\mathrm{1M} \to \mathrm{1k})
\times 10^{\mathrm{att. \, fac.}}
\times (\mathrm{LEBT} \to \mathrm{P2DT})
\times (\mathrm{P2DT} \to \mathrm{F0})
\times \mathrm{F0_{acc.}}
```
<!--
- $I_{\mathrm{pps}}$: beam intensity in particles per second
- $\mathrm{LEBT}_{\mathrm{peak}}$: peak current measured at LEBT
- $\mathrm{PulseWidth}$: width of the beam pulse (s)
- $\mathrm{Repetition}$: pulse repetition frequency (Hz)
- $Q$: charge state
- $e$: elementary charge
- $(\mathrm{1M} \to \mathrm{1k})$: scale conversion factor
- $10^{\mathrm{att. \, fac.}}$: attenuation factor
- $(\mathrm{LEBT} \to \mathrm{P2DT})$: conversion efficiency
- $(\mathrm{P2DT} \to \mathrm{F0})$: conversion efficiency
- $\mathrm{F0_{acc.}}$: acceptance at F0
-->

---

## 용어/단위 정의

- **LEBT**: LEBT 구간 빔 모니터에서 측정되는 전류(노션 메모 기준 “A 단위로 보는 것이 맞을 가능성” 언급 포함).
- **Mean beam intensity**: 특정 run 시간 구간에서의 LEBT 전류를 시간가중 평균한 값. (코드에서는 `LEBT_mean`)
- **Integral beam intensity**: run 시간 구간에서 LEBT 전류를 시간에 대해 적분한 값. (코드에서는 `LEBT_integral`, 단위 A·s)
- **Live time**: DAQ가 실제로 이벤트를 기록한 비율. scaler의 `hwtriga/hwtrigb`로 평가.

---

## 데이터 소스 (노션 + 레포 기준)

- **빔 모니터 원시 로그(CSV)**: `misc/Beam/<mmdd>/...csv`
  - 예: `LEBT-DIAG_ACCT001_Current.csv`, `P2DT-DIAG_ACCT001_Current.csv`
  - (추가로 FC 모니터 로그도 병합 가능: `KoBRA-DIAG_FC001_Current_1K.csv` 등)
- **run 시작/끝 시간표**: `misc/midcheck_summary.csv` (코드에서 RunTimeData로 읽음)
- **분석에서 사용되는 run별 beam 요약 테이블**: `misc/beam_time.csv`
- **scaler 요약/Live time**: `scaler/scaler.csv` 또는 `misc/scaler_ext_<isotope>.csv`(분석/플로팅용)

---

## 레벨 A: “분석 코드에서 실제로 쓰는 beam intensity(LEBT_mean)” 산출법

### 1) 개요

1. 빔 모니터 로그들을 날짜별로 읽고, `datetime` 기준으로 병합
2. `midcheck_summary.csv`의 run 시작/끝 시간을 가져옴
3. 그 시간 구간에서 LEBT 시계열을 **시간 간격으로 가중**하여 평균(`LEBT_mean`)과 표준편차(`LEBT_std`)를 계산
4. `LEBT_integral = LEBT_mean × (run duration [s])`로 적분값을 계산
5. 위 결과를 `misc/beam_time.csv`에 저장

### 2) 구현(레포 코드 기준)

- **빔 모니터 병합/시간가중 평균**: `scripts/beam_data.py`
  - 날짜별 CSV를 읽어 `beam_data_merged.pkl`로 캐시
  - run 구간 평균/적분을 계산해 `misc/beam_time.csv` 생성
- **C++에서 run별 beam 조회**: `macros/kobra_beam.cxx`, `macros/kobra_beam.hxx`
  - `misc/beam_time.csv`를 읽어서 `GetMeanBeamIntensity(run)` → `LEBT_mean` 반환

분석에서 beam intensity가 실제로 사용되는 지점(모멘텀 분포/스케일링 예):

```1160:1168:macros/kobra.cxx
  Double_t scale = 1. / (GetPPACEff(2) * GetPPACEff(3) * KOBRAScaler::GetInstance()->GetLiveTime(runNs));
  scale *= (1. / KOBRAF0::GetInstance()->GetRatio(runNs.front()));
  scale *= (1. / KOBRABeam::GetInstance()->GetMeanBeamIntensity(runNs.front()));
```

### 3) 재현(로컬에서 beam_time.csv 다시 만들기)

1) `misc/Beam/<mmdd>/` 아래에 빔 모니터 CSV들이 준비되어 있어야 합니다.
2) `misc/midcheck_summary.csv`에 run별 start/stop time이 있어야 합니다.

그 다음 아래 함수로 `misc/beam_time.csv`를 재생성합니다(파이썬 쪽):

- `BeamData.get_beam_average_csv(runtime_csv_path='misc/midcheck_summary.csv')`

---

## 레벨 A’(보조): Scaler로부터 “Rate / Live time”을 계산하는 방법

Beam intensity 자체(LEBT)는 scaler가 아니라 **빔 모니터 로그**에서 오지만,
최종 물리량(예: differential rate, cross section) 계산에는 scaler의 **시간/라이브타임**이 함께 들어갑니다.

- **scaler 추출(C++)**: `KOBRA::ExtractScalerInfo()`가 `TScalerData`를 CSV 형태로 저장
- **Live time 계산(C++)**: `KOBRAScaler::GetLiveTime()`가 `hwtriga/hwtrigb` 사용

```84:90:macros/kobra_scaler.cxx
double KOBRAScaler::GetLiveTime(int run) {
    double hwtriga = Get(run, "hwtriga");
    double hwtrigb = Get(run, "hwtrigb");
    if (hwtriga == 0 || hwtrigb == 0) return 0;
    return hwtriga / hwtrigb;
}
```

파이썬 측면에서 scaler rate(초당 변화량)를 계산하는 예:

```35:44:scripts/scaler.py
        rate = self.all_df[col_name].diff() / self.all_df["ts"].diff()
        return rate
```

---

## 레벨 B: LEBT 전류(A) → pps(입자/초) 변환(노션 방법론 요약)

노션의 “Beam flux” 페이지에는 **전류를 pps로 바꾸는 전체 변환 사슬**이 정리되어 있습니다. 핵심 요소는 다음과 같습니다.

- **펄스 평균 전류**: $ I_{avg} = I_{peak} \times \text{PulseWidth} \times \text{Repetition} $
  - 예: pulse width 5 ms, repetition 20 Hz → duty = 0.1
- **전하상태 보정**: 이온 전하 $Q$ (예: Ar\(^{8+}\)이면 $Q=8$)
- **전류→입자수 변환**: $ \text{pps} = I_{avg} / (Q e) $
- **계측 스케일 변환**: 1M↔1K 같은 계측 range 변환 계수(노션에 수치가 기록됨)
- **전달/전송 보정(Transmission)**:
  - LEBT → P2DT, P2DT → F0 등의 전송 계수
  - attenuation 설정(예: -2, -4, …) 보정
  - F0 acceptance(beam image 기반) 보정

즉, “총 F0 beam intensity(pps)”는 다음 형태로 구성됩니다(노션 식을 구조만 보존해 요약).

$$
I_{\mathrm{F0}}[\mathrm{pps}]
= \frac{I_{\mathrm{LEBT,peak}}\cdot \text{PulseWidth}\cdot \text{Repetition}}{Qe}
\times (\text{range conv}) \times (\text{attenuation}) \times (\text{transmissions}) \times (\text{F0 acc.})
$$

> 실무 팁: 코드베이스에서 `misc/beam_time.csv`는 **LEBT 전류 기반(레벨 A)**이므로,
> pps가 필요하면 위 변환(레벨 B)을 별도 레이어로 적용해야 합니다.

---

## 불확도(에러) 처리 가이드(노션 로그 기반)

노션 로그에는 beam intensity(LEBT) 값이 시간에 따라 분포를 가지므로,
**통계적 분산(표준편차)을 에러바로 포함**하는 접근이 언급되어 있습니다.

레포에서도 `scripts/beam_data.py`가 `LEBT_std`를 함께 계산/출력하며,
run duration에 대해 적분값의 불확도는 대략적으로

- **Mean의 불확도**: $ \sigma(I)$ (시간가중 표준편차 또는 표준오차 정책 선택)
- **Integral의 불확도**: $ \sigma(\int I dt) \approx \sigma(I)\cdot T $

로 전파할 수 있습니다(정확한 정책은 분석 목적에 맞춰 결정).

---

## 체크리스트(새 연구원이 따라할 때)

- **입력 준비**
  - `misc/Beam/<mmdd>/`에 LEBT/P2DT/FC 로그가 있는지
  - `misc/midcheck_summary.csv`에 run start/stop이 있는지
- **레벨 A 산출**
  - `scripts/beam_data.py`로 `misc/beam_time.csv` 생성
  - C++에서 `KOBRABeam::GetMeanBeamIntensity(run)`로 LEBT_mean이 정상 조회되는지
- **분석 연결**
  - 모멘텀 분포/단면적 스케일링에 `1/LEBT_mean`이 들어가는지 확인
  - Live time(`hwtriga/hwtrigb`)과 F0 ratio가 함께 들어가는지 확인
- **레벨 B 변환(필요 시)**
  - pulse duty, Q, transmission, attenuation, F0 acceptance를 정의/검증
  - 단위(A ↔ pA ↔ pps) 변환이 일관적인지 재확인

---

## 출처/참조

- **Notion(정의/변환 사슬)**: `Beam flux` — `https://www.notion.so/214dfd46b21d80058303fc7e4b999cf7`
- **Notion(스케일러 추출/비교 흐름)**: `Log @2025년 5월 19일 (Scaler information)` — `https://www.notion.so/1f8dfd46b21d80989348ed7eafff992b`
- **레포(구현)**
  - `scripts/beam_data.py`: run 구간 시간가중 평균/적분으로 `beam_time.csv` 생성
  - `macros/kobra_beam.cxx/.hxx`: `beam_time.csv`에서 run별 `LEBT_mean`, `LEBT_integral` 조회
  - `macros/kobra_scaler.cxx/.hxx`, `scripts/scaler.py`: Live time/rate 산출
