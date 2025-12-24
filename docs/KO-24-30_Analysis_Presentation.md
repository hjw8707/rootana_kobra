# KO-24-30 실험 분석 소개
## Analysis Log 정리

---

## 목차

1. 실험 개요
2. 주요 분석 항목
3. PPAC (Parallel Plate Avalanche Counter) 분석
4. TSum Cut 개선
5. PPAC Efficiency 분석
6. 주요 발견사항
7. 향후 계획

---

## 1. 실험 개요

### KO-24-30 실험
- **목적**: 중이온 빔 실험 분석
- **주요 타겟**: 18O, 19O, 20O, 21O, 22O, 24Ne, 25Ne, 26Ne
- **실험 설정**: CS (Charge State) 및 MDIS (Magnetic Dipole) runs
- **분석 기간**: 2024년 7월 ~ 2025년 12월

### 주요 검출기
- **PPAC**: F1 Up, F1 Up X, F2 Down, F3 Up, F3 Down
- **SSD**: Silicon Strip Detector
- **Plastic**: Plastic Scintillator

---

## 2. 주요 분석 항목

### 2.1 데이터 품질 관리
- TSum (Timing Sum) Cut 개선
- TDC (Time-to-Digital Converter) 분포 분석
- PPAC Efficiency 측정

### 2.2 기하학적 수용각 분석
- X/Y 위치 분포 분석
- Slit 중심 위치 문제 확인
- Geometrical acceptance 평가

### 2.3 빔 특성 분석
- LEBT (Low Energy Beam Transport) 분석
- Beam extraction 개념 정리
- Transmission 분석

---

## 3. PPAC 분석

### 3.1 PPAC 개요
**PPAC (Parallel Plate Avalanche Counter)**
- 빔 위치 및 타이밍 측정
- 다중 PPAC 시스템으로 빔 추적

**주요 PPAC 위치:**
- **F1 Up PPAC**: 상류 위치
- **F1 Up PPACX**: X 방향 전용
- **F2 Down PPAC**: 중간 위치
- **F3 Up PPAC**: 하류 상부
- **F3 Down PPAC**: 하류 하부

---

## 4. TSum Cut 개선

### 4.1 TSum Cut 개선 배경
- **기존 방법**: 고정된 범위 사용
- **개선 방법**: 5-sigma 규칙 적용
  - Mean ± 5σ 범위 사용
  - Run별, 설정별 동적 Cut 적용

### 4.2 CS Runs TSum 값 (Run 251 예시)

| PPAC | 방향 | Mean | Sigma | 5σ 범위 |
|------|------|------|-------|---------|
| F1 Up | TX | 3883.1 | 63.5 | 3566-4201 |
| F1 Up | TY | 5800.8 | 129.1 | 5155-6447 |
| F2 Down | TX | 2934.9 | 34.8 | 2761-3109 |
| F2 Down | TY | 2862.3 | 48.4 | 2620-3104 |
| F3 Up | TX | 3242.6 | 37.5 | 3055-3430 |
| F3 Up | TY | 3233.5 | 39.7 | 3035-3432 |
| F3 Down | TX | 3655.2 | 42.3 | 3444-3867 |
| F3 Down | TY | 3703.3 | 44.2 | 3482-3924 |

### 4.3 MDIS Runs TSum 값 (Run 656 예시)

| PPAC | 방향 | Mean | Sigma | 5σ 범위 |
|------|------|------|-------|---------|
| F1 Up | TX | 3857.6 | 51.0 | 3602-4113 |
| F1 Up | TY | 5788.8 | 128.0 | 5149-6429 |
| F2 Down | TX | 2931.6 | 36.1 | 2751-3112 |
| F2 Down | TY | 2845.9 | 49.1 | 2601-3091 |
| F3 Up | TX | 3229.4 | 46.1 | 2999-3460 |
| F3 Up | TY | 3222.4 | 45.8 | 2993-3451 |
| F3 Down | TX | 3630.1 | 52.6 | 3367-3893 |
| F3 Down | TY | 3677.9 | 54.1 | 3407-3948 |

---

## 5. PPAC Efficiency 분석

### 5.1 Efficiency 정의
**Efficiency = (TSum Cut 후 PPAC Hit) / (SSD 또는 F3 Plastic Hit 이벤트)**

### 5.2 CS All Runs Efficiency

| PPAC | Efficiency |
|------|------------|
| F1 Up PPAC | 83.0% |
| F1 Up PPACX | 89.6% |
| F2 Down PPAC | 86.0% |
| F3 Up PPAC | 86.0% |
| F3 Down PPAC | 85.6% |

### 5.3 MDIS All Runs Efficiency

| PPAC | Efficiency |
|------|------------|
| F1 Up PPAC | 63.9% |
| F1 Up PPACX | 76.3% |
| F2 Down PPAC | 85.5% |
| F3 Up PPAC | 86.6% |
| F3 Down PPAC | 85.8% |

**관찰**: CS runs에서 F1 Up PPAC의 효율이 MDIS runs보다 높음

---

## 6. 주요 발견사항

### 6.1 F1 Up PPAC 분석

#### TYSum에서 95000~100000 범위의 Peak
- **원인**: Anode 타이밍의 영향
- **관찰**: TYSum = TY1 + TY2 - 2×TA
- **결론**: Anode가 해당 peak의 원인임을 확인

#### X만 사용 시 효율 향상
- Y와 함께 사용: 기본 효율
- **X만 사용**: 8% 추가 효율 향상

### 6.2 F3 Up PPAC Y Cut 문제

#### 문제점
- Y = -30 근처에서 잘림
- Commissioning runs (459~469, 538~548)에서도 동일한 현상

#### 원인 분석
- **Slit 중심 위치 문제**: Displaced slit center (+11 mm)
- Commissioning runs에서도 확인되어 슬릿 문제로 확신

#### F3 Up PPAC Anode < 56000 이벤트
- TX, TYSum cut 적용 후에도 일부 이벤트 남음
- 추가 분석 필요

---

## 7. TDC 분포 분석

### 7.1 F2 Down PPAC TDC 분포 (CS Runs)
- **관찰**: 이상한 형태의 분포
- **가설**: 자기 자신이 트리거이기 때문에 발생 가능
- **비교**: F3 Down PPAC과 비교 분석

### 7.2 F3 Up PPAC TDC 분포
- Commissioning runs에서도 유사한 패턴
- F2 Down PPAC과의 coincidence 확인 필요

---

## 8. CS Setting별 Efficiency

### 8.1 18O Runs (Run 251)
- F1 Up PPACX: 95.2%
- F2 Down PPAC: 99.6%
- F3 Up PPAC: 27.8%
- F3 Down PPAC: 21.6%

### 8.2 19O Runs (Run 259)
- F1 Up PPACX: 95.4%
- F2 Down PPAC: 99.2%
- F3 Up PPAC: 27.7%
- F3 Down PPAC: 20.8%

### 8.3 20O Runs (Run 304)
- F1 Up PPACX: 90.8%
- F2 Down PPAC: 99.8%
- F3 Up PPAC: 19.9%
- F3 Down PPAC: 14.7%

### 8.4 21O Runs (Run 316)
- F1 Up PPACX: 77.1%
- F2 Down PPAC: 99.5%
- F3 Up PPAC: 13.1%
- F3 Down PPAC: 9.5%

### 8.5 22O Runs (Run 223)
- F1 Up PPACX: 52.2%
- F2 Down PPAC: 98.9%
- F3 Up PPAC: 5.2%
- F3 Down PPAC: 3.8%

### 8.6 24Ne Runs (Run 410)
- F1 Up PPACX: 92.6%
- F2 Down PPAC: 98.4%
- F3 Up PPAC: 29.5%
- F3 Down PPAC: 21.4%

### 8.7 25Ne Runs (Run 420)
- F1 Up PPACX: 70.8%
- F2 Down PPAC: 99.2%
- F3 Up PPAC: 15.9%
- F3 Down PPAC: 12.2%

### 8.8 26Ne Runs (Run 425)
- F1 Up PPACX: 35.6%
- F2 Down PPAC: 98.7%
- F3 Up PPAC: 5.6%
- F3 Down PPAC: 4.2%

**관찰**:
- F2 Down PPAC는 모든 설정에서 높은 효율 유지 (~99%)
- F1 Up PPACX와 F3 PPACs는 질량수에 따라 효율 변화
- 무거운 동위원소일수록 F3 PPACs의 효율 감소

---

## 9. TSum 값의 설정별 변화

### 9.1 F1 Up PPAC TSum 변화량

| 양 | 변화량 | 최소 Mean | 최대 Mean | 평균 Mean | 표준편차 |
|----|--------|-----------|-----------|-----------|----------|
| TXSum | 53.9 | 3843.9 | 3897.7 | 3879.0 | 19.0 |
| TYSum | 64.1 | 5759.2 | 5823.3 | 5794.8 | 24.2 |

### 9.2 F2 Down PPAC TSum 변화량

| 양 | 변화량 | 최소 Mean | 최대 Mean | 평균 Mean | 표준편차 |
|----|--------|-----------|-----------|-----------|----------|
| TXSum | 32.1 | 2915.1 | 2947.3 | 2934.0 | 9.0 |
| TYSum | 45.5 | 2837.2 | 2882.7 | 2861.0 | 13.6 |

### 9.3 F3 Up PPAC TSum 변화량

| 양 | 변화량 | 최소 Mean | 최대 Mean | 평균 Mean | 표준편차 |
|----|--------|-----------|-----------|-----------|----------|
| TXSum | 37.6 | 3207.4 | 3245.0 | 3235.9 | 12.3 |
| TYSum | 26.5 | 3209.6 | 3236.1 | 3228.2 | 8.2 |

### 9.4 F3 Down PPAC TSum 변화량

| 양 | 변화량 | 최소 Mean | 최대 Mean | 평균 Mean | 표준편차 |
|----|--------|-----------|-----------|-----------|----------|
| TXSum | 75.2 | 3587.0 | 3662.2 | 3638.6 | 27.5 |
| TYSum | 81.7 | 3627.3 | 3708.9 | 3683.8 | 30.1 |

**관찰**: F3 Down PPAC가 가장 큰 변화량을 보임

---

## 10. 주요 기술적 개선사항

### 10.1 TSum Cut 개선
- **이전**: 고정된 범위 사용
- **개선**: 5-sigma 규칙 적용
  - Mean ± 5σ 범위
  - Run별, 설정별 동적 적용
  - `par_set.py`에 구현

### 10.2 데이터 분석 체계화
- Analysis Log 데이터베이스 구축
- 날짜별, 태그별 분류
- 캘린더 뷰 제공

### 10.3 정기 미팅 체계
- 주간 Analysis Meeting
- To-do 리스트 관리
- 논의사항 기록 및 추적

---

## 11. 현재 진행 중인 작업

### 11.1 To-do List (2025년 12월 기준)

#### 완료된 항목
- [x] A/Q and Z resolution 정량적 분석 (SSD 및 Plastic)
- [x] LEBT vs Yield 분석 (X축 에러 포함)
- [x] Momentum distribution 보정 방법 문서화

#### 진행 중인 항목
- [ ] 각 설정(Target nucleus)별 개별 분석
- [ ] Momentum distribution에 Systematic error 포함
- [ ] F0 Beam transmission log note 확인

### 11.2 논의 중인 사항

#### F1 TDC 분포
- 90000~100000 근처 이벤트 확인 필요
- F3 Up과의 관련성 조사

#### F2 TDC 분포
- 이상한 분포 형태 확인
- Commissioning runs와 비교
- Trigger 생성 방법 검토

#### PPAC Efficiency
- SSD 기준 효율 측정
- 설정별, Z별 효율 분석

---

## 12. 향후 계획

### 12.1 단기 계획
1. **각 설정별 개별 분석 완료**
   - Target nucleus별 상세 분석
   - 이후 전체 통합 분석

2. **Systematic Error 평가**
   - Momentum distribution에 포함
   - Beam rate 관련 systematic error

3. **기하학적 수용각 정량화**
   - X/Y 분포 분석
   - Acceptance 계산

### 12.2 중장기 계획
1. **Technical Report 작성**
   - Google Docs 체계적 정리
   - 분석 방법론 문서화

2. **논문 작성**
   - 실험 결과 정리
   - 물리적 해석

3. **데이터 분석 자동화**
   - 분석 스크립트 개선
   - 재현 가능한 분석 파이프라인 구축

---

## 13. 주요 성과 요약

### 13.1 분석 방법론 개선
- ✅ TSum Cut 개선 (5-sigma 규칙)
- ✅ PPAC Efficiency 체계적 측정
- ✅ 설정별 상세 분석

### 13.2 주요 발견
- ✅ F3 Up PPAC Y cut 문제 원인 규명 (Slit 위치)
- ✅ F1 Up PPAC Anode 타이밍 영향 확인
- ✅ 설정별 Efficiency 변화 패턴 파악

### 13.3 데이터 품질 향상
- ✅ TDC 분포 이상 현상 확인
- ✅ Commissioning runs와 비교 분석
- ✅ Trigger 효과 분석

---

## 14. 참고 자료

### 14.1 분석 코드
- `macros/ppac_eff_analysis.C`: PPAC Efficiency 분석
- `pars/par_set.py`: TSum Cut 설정

### 14.2 문서
- Analysis Log: 노션 데이터베이스
- Technical Report: Google Docs
- Meeting Notes: 정기 미팅 기록

### 14.3 데이터
- CS Runs: Charge State runs
- MDIS Runs: Magnetic Dipole runs
- Commissioning Runs: 459~469, 538~548

---

## 15. 결론

### KO-24-30 실험 분석의 주요 성과

1. **체계적인 분석 체계 구축**
   - Analysis Log 데이터베이스
   - 정기 미팅 및 To-do 관리

2. **데이터 품질 개선**
   - TSum Cut 개선
   - PPAC Efficiency 정량화

3. **주요 문제점 규명**
   - F3 Up PPAC Y cut 문제 (Slit 위치)
   - TDC 분포 이상 현상

4. **향후 연구 방향 제시**
   - 설정별 개별 분석
   - Systematic error 평가
   - 논문 작성 준비

---

## 감사합니다

**질문이 있으시면 언제든지 말씀해 주세요!**

---

## 부록: 주요 수치 요약

### PPAC Efficiency (CS All)
- F1 Up PPAC: 83.0%
- F1 Up PPACX: 89.6%
- F2 Down PPAC: 86.0%
- F3 Up PPAC: 86.0%
- F3 Down PPAC: 85.6%

### PPAC Efficiency (MDIS All)
- F1 Up PPAC: 63.9%
- F1 Up PPACX: 76.3%
- F2 Down PPAC: 85.5%
- F3 Up PPAC: 86.6%
- F3 Down PPAC: 85.8%

### TSum Cut 범위 (CS Runs 예시)
- F1 Up TX: 3566-4201
- F1 Up TY: 5155-6447
- F2 Down TX: 2761-3109
- F2 Down TY: 2620-3104
- F3 Up TX: 3055-3430
- F3 Up TY: 3035-3432
- F3 Down TX: 3444-3867
- F3 Down TY: 3482-3924

