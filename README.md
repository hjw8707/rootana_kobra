# KoBRA 데이터 분석을 위한 Rootana 기반 코드

**저자:** 황종원 (기초과학연구원, 희귀핵연구단)
**이메일:** <jwhwang@ibs.re.kr>
**최종 수정일:** 2025년 5월 20일 (버전 정보는 필요에 따라 업데이트하세요)

## 1. 개요

본 프로젝트는 한국형 중이온가속기 라온(RAON)의 KoBRA(Korea Broad acceptance Recoil spectrometer Array) 실험 데이터를 분석하기 위해 개발된 소프트웨어 패키지입니다. MIDAS 패키지 내의 ROOTANA 프레임워크를 기반으로 구축되었습니다. KoBRA에서 생성된 원시 데이터를 처리하여 물리 분석에 필요한 ROOT 파일을 생성하고, 이를 분석하는 다양한 도구와 클래스를 제공합니다.

## 2. 주요 구성 요소 및 기능

본 패키지는 다음과 같은 주요 구성 요소로 이루어져 있습니다.

### 2.1. `rakobra`

* **역할:** KoBRA 실험에서 수집된 원시 데이터(.mid 파일 등)로부터 분석용 ROOT 파일을 생성하는 프로그램입니다.
* **주요 기능:** 이벤트 빌딩, 원시 데이터 디코딩, 기본적인 물리량 계산 및 TTree 형태로 ROOT 파일에 저장합니다.

### 2.2. `rakobra_server`

* **역할:** 온라인 데이터 분석 및 모니터링을 위한 서버 프로그램입니다.
* **주요 기능:** 실험 중 실시간으로 데이터를 수신하여 `rakobra`와 유사한 처리 과정을 거치거나, 미리 정의된 히스토그램을 채우고 웹 인터페이스 등을 통해 시각화하여 실험 상태를 모니터링할 수 있도록 지원합니다. (구체적인 기능은 실제 구현에 따라 다를 수 있습니다.)

### 2.3. `KOBRA` 클래스 (`macros/kobra.cxx`, `macros/kobra.hh`)

* **역할:** `rakobra` 또는 유사한 과정을 통해 생성된 ROOT 파일을 분석하기 위한 C++ 클래스입니다. ROOT 환경 내에서 매크로 형태로 사용됩니다.
* **주요 기능:**
  * 다양한 실험 런(run) 데이터 로드 및 관리 (예: 18O, 19O, 20O 등 동위원소별 런 정보)
  * 입자 식별(PID) 다이어그램 생성 (예: ΔE-E, A/Q-Z 등)
  * 특정 입자에 대한 게이트(TCutG) 정의 및 적용
  * 운동량 분포, 각도 분포 등 물리량 히스토그램 생성 및 피팅
  * 스케일러 데이터(`TScalerData`) 접근 및 활용
  * PPAC 효율성 계산, 라이브타임 계산 등
  * 단면적(Cross-section) 분석 및 운동량 분포(Momentum distribution) 분석을 위한 헬퍼 함수 제공

### 2.4. `scripts/scaler.py`

* **역할:** KoBRA 실험의 스케일러(scaler) 데이터를 처리하고 시각화하는 Python 스크립트입니다.
* **주요 기능:**
  * 스케일러 데이터 CSV 파일 (예: `misc/scaler_ext_{isotope}.csv`) 로드
  * 시간에 따른 각 채널의 변화율(rate) 계산
  * Matplotlib을 사용한 데이터 시각화 (예: `reft`에 따른 `ppac_rate` 등)
  * 런(run)별 총량 및 평균 변화율 요약 정보 출력

## 3. 요구사항

* **ROOT:** C++ 기반 데이터 분석 프레임워크 ([https://root.cern/](https://root.cern/))
* **MIDAS:** 데이터 수집 시스템 ([https://midas.triumf.ca/](https://midas.triumf.ca/))
  * **ROOTANA:** MIDAS 내의 ROOT 기반 분석 프레임워크
* **Python 3.x**
  * **pandas:** 데이터 분석 라이브러리
  * **matplotlib:** 데이터 시각화 라이브러리
  * **numpy:** 수치 계산 라이브러리
* C++ 컴파일러 (g++ 등)

## 4. 설치

1. **MIDAS 및 ROOTANA 설치:**
    * MIDAS 공식 문서에 따라 시스템에 MIDAS를 설치합니다.
    * ROOTANA가 MIDAS 환경 내에서 올바르게 설정되었는지 확인합니다.
2. **ROOT 설치:**
    * ROOT 공식 웹사이트에서 시스템에 맞는 버전을 다운로드하여 설치하고, 환경 변수(`ROOTSYS`, `PATH`, `LD_LIBRARY_PATH`)를 설정합니다.
3. **Python 라이브러리 설치:**

    ```bash
    pip install pandas matplotlib numpy
    ```

4. **본 프로젝트 빌드:**
    * `rakobra` 및 `rakobra_server` 컴파일: 제공된 `Makefile` 또는 빌드 스크립트를 사용합니다. (빌드 방법 명시 필요)

      ```bash
      # 예시 (실제 빌드 명령어는 다를 수 있습니다)
      make
      ```

    * `KOBRA` 클래스: 일반적으로 ROOT 매크로로 실행되므로 별도의 빌드 과정이 필요 없을 수 있으나, 공유 라이브러리(`.so`) 형태로 빌드하여 사용할 수도 있습니다.

## 5. 사용법

### 5.1. `rakobra`: ROOT 파일 생성

```bash
# 예시 (실제 실행 명령어 및 옵션은 다를 수 있습니다)
./rakobra -i <input_data_file.mid> -o <output_file.root>
```

또는 `ana` 디렉토리의 스크립트를 사용하여 실행할 수 있습니다.

### 5.2. `KOBRA` 클래스: ROOT 파일 분석

ROOT 프롬프트에서 `KOBRA` 클래스를 사용하는 매크로를 실행합니다.

```cpp
// 예시: my_analysis.C
void my_analysis() {
    // KOBRA 클래스 헤더 및 소스 로드 (필요시)
    // gSystem->Load("libKOBRA.so"); // 만약 공유 라이브러리로 빌드했다면
    // gROOT->ProcessLine(".L macros/kobra.cxx+"); // JIT 컴파일

    KOBRA k(KOBRA::Expt::Phys, 251); // 예: 18O 실험, 런 251번 로드
    // 또는 여러 런 로드
    // std::vector<int> runs = KOBRA::o19; // 19O 모든 런
    // KOBRA k(KOBRA::Expt::Phys, runs);

    k.SetAlias(); // 물리량 Alias 설정
    k.AddCuts("cut"); // 게이트 파일들이 있는 디렉토리 지정 (예: ./cut)

    // PID 그리기 (0: 기본, 1: 카운트 표시, 2: 비율 표시)
    k.DrawPIDC(1);

    // 특정 동위원소 (예: "o19")의 운동량 분포 그리기
    // k.DrawMomDist("o19");

    // 결과 저장
    // gPad->GetCanvas()->SaveAs("pid_o18_run251.png");
}
```

ROOT 실행:

```bash
root -l my_analysis.C
```

### 5.3. `scripts/scaler.py`: 스케일러 데이터 분석

```bash
python scripts/scaler.py <isotope_name>
# 예시: 18O 데이터의 경우 (misc/scaler_ext_18o.csv 파일이 존재해야 함)
python scripts/scaler.py 18o
```

스크립트는 해당 동위원소의 스케일러 데이터를 읽어 `Run Summary`를 터미널에 출력합니다. 코드 내에서 주석 처리된 부분을 해제하여 특정 런의 `ppac_rate` 그래프 등을 그릴 수 있습니다.

## 6. 데이터 파일

* **KoBRA 원시 데이터:** `.mid` 형식 (MIDAS 표준)
* **분석용 ROOT 파일:** `rakobra`를 통해 생성되며, `TTree` 형태로 이벤트 데이터 저장. `TScalerData` 객체도 포함될 수 있음.
* **스케일러 데이터:** `misc/scaler_ext_{isotope}.csv` 형식. `run, ts, ppac, f2pl, f3pl, hwtriga, hwtrigb, refp, reft` 등의 컬럼을 가짐.
* **A/Q vs reft 데이터:** `misc/aoq_reft.csv` (예시)
* **PID 게이트 파일:** `cut/` 디렉토리 내 텍스트 파일. 각 파일은 특정 동위원소에 대한 A/Q, Z 좌표를 정의.

## 7. 기여하기

버그를 발견하거나 개선 사항이 있다면 언제든지 이슈를 등록하거나 Pull Request를 보내주세요.

## 8. 라이선스

(프로젝트에 적용된 라이선스를 명시하세요. 예: MIT License, GPL 등)
