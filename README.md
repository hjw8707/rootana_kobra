# KoBRA 데이터 분석을 위한 Rootana 기반 코드

**저자:** 황종원 (기초과학연구원, 희귀핵연구단)

**이메일:** <jwhwang@ibs.re.kr>

**최종 수정일:** 2025년 12월 24일

## 1. 개요

본 프로젝트는 한국형 중이온가속기 라온(RAON)의 KoBRA(Korea Broad acceptance Recoil spectrometer Array) 실험 데이터를 분석하기 위한 **ROOTANA 기반 분석 코드**입니다.

- **오프라인 분석**: MIDAS raw data(`.mid`, `.mid.lz4`) → `rakobra` → `output000XXXXX.root` 생성
- **온라인 모니터링**: `rakobra_server`가 JSROOT(`THttpServer`) 기반 웹 모니터링 서버를 띄움
- **후처리/물리분석**: `macros/kobra.cxx` / `macros/kobra.hxx`의 `KOBRA` 클래스로 ROOT 파일 분석

## 2. 주요 구성 요소 및 기능

본 패키지는 다음과 같은 주요 구성 요소로 이루어져 있습니다.

### 2.1. `rakobra`

- **역할**: KoBRA 실험 원시 데이터(`.mid`, `.mid.lz4`)를 읽어 분석용 ROOT 파일을 생성합니다.
- **출력**: 실행한 디렉토리에 `output000XXXXX.root` 형태로 생성됩니다(스크립트에서 이 파일을 이동/정리합니다).

### 2.2. `rakobra_server`

- **역할**: 온라인 데이터 분석 및 모니터링 서버입니다.
- **주요 기능**: `THttpServer("http:9091")`로 웹 서버를 열고, `online.htm`을 기본 페이지로 사용합니다.
  - 기본 접속 주소: `http://localhost:9091`
  - `MIDASSYS/resources`를 웹에서 접근 가능하도록 location을 추가합니다.

### 2.3. `KOBRA` 클래스 (`macros/kobra.cxx`, `macros/kobra.hxx`)

- **역할**: `rakobra`로 생성된 ROOT 파일을 분석하기 위한 C++ 클래스입니다(주로 ROOT 매크로로 사용).
- **주요 기능**:
  - 다양한 실험 런(run) 데이터 로드 및 관리 (예: 18O, 19O, 20O 등 동위원소별 런 정보)
  - 입자 식별(PID) 다이어그램 생성 (예: ΔE-E, A/Q-Z 등)
  - 특정 입자에 대한 게이트(TCutG) 정의 및 적용
  - 운동량 분포, 각도 분포 등 물리량 히스토그램 생성 및 피팅
  - 스케일러 데이터(`TScalerData`) 접근 및 활용
  - PPAC 효율성 계산, 라이브타임 계산 등
  - 단면적(Cross-section) 분석 및 운동량 분포(Momentum distribution) 분석을 위한 헬퍼 함수 제공

### 2.4. `scripts/scaler.py`

- **역할**: KoBRA 스케일러 CSV(`misc/scaler_ext_{isotope}.csv`)를 로드해 **run 요약(총량/평균 rate)**를 출력하고, 필요 시 플롯을 그립니다.

## 3. 요구사항

- **CMake**: 3.10+
- **C++ 컴파일러**: clang++ 또는 g++
- **ROOT**: CMake에서 `find_package(ROOT ...)`가 가능해야 함
- **MIDAS + ROOTANA**: 헤더/라이브러리 경로를 환경변수로 제공해야 함
  - `MIDASSYS`: MIDAS 설치 루트
  - `ROOTANASYS`: ROOTANA 설치 루트(또는 ROOTANA 빌드/설치 디렉토리)
  - `ROOTSYS`: ROOT 설치 루트(이 프로젝트의 `CMakeLists.txt`는 `ROOTSYS/etc/cmake`를 참조)
- **Python 3.x (선택)**: `scripts/` 사용 시
  - `pandas`, `matplotlib`, `numpy`

## 4. 설치

아래는 **macOS 기준**으로 “깨끗하게” 설치/빌드하는 가장 재현 가능한 방법입니다.

### 4.1. ROOT 설치 및 환경 설정

- **옵션 A (권장: 패키지 매니저)**: Homebrew 또는 conda-forge로 ROOT 설치
- **옵션 B**: 소스 빌드/공식 바이너리 설치

어떤 방식이든, 터미널에서 아래가 동작해야 합니다.

```bash
root-config --version
root -l -b -q
```

또한 이 프로젝트는 `CMakeLists.txt`에서 `ROOTSYS`를 참조하므로, 아래 중 하나가 필요합니다.

- **권장**: ROOT 설치가 제공하는 `thisroot.sh`를 실행해 환경변수를 설정
- **또는**: `ROOTSYS`를 직접 export

### 4.2. MIDAS / ROOTANA 설치 및 환경 설정

이 코드는 ROOTANA(`rootana` 라이브러리)와 MIDAS(`midas` 라이브러리)에 링크합니다. 빌드 전에 아래 환경변수가 설정되어 있어야 합니다.

```bash
export MIDASSYS=/path/to/midas
export ROOTANASYS=/path/to/rootana
export ROOTSYS=/path/to/root
```

참고:

- macOS에서 동적 라이브러리 로딩 문제가 있으면 `DYLD_LIBRARY_PATH`가 필요할 수 있습니다.
- ROOTANA/MIDAS를 직접 빌드했다면 `ROOTANASYS/lib`, `MIDASSYS/lib` 아래에 라이브러리가 존재하는지 확인하세요.

### 4.3. (선택) Python 의존성 설치

```bash
python3 -m pip install --upgrade pip
python3 -m pip install pandas matplotlib numpy
```

### 4.4. 본 프로젝트 빌드(CMake)

레포 루트에서:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

빌드 산출물(기본값):

- 실행 파일: `build/rakobra`, `build/rakobra_server`, `build/midcheck`
- 라이브러리: `build/libkobra.*` 및 ROOT dictionary 관련 파일들

**중요**: `ana.sh`/`batch.sh`는 기본적으로 `./rakobra`를 호출합니다. 아래 중 한 가지를 선택하세요.

- **옵션 A (권장)**: 실행할 때 `build/` 아래 바이너리를 직접 사용
- **옵션 B**: 스크립트 호환을 위해 루트에 심볼릭 링크 생성

```bash
ln -sf build/rakobra ./rakobra
ln -sf build/rakobra_server ./rakobra_server
ln -sf build/midcheck ./midcheck
```

## 5. 사용법

### 5.1. `rakobra`: ROOT 파일 생성

```bash
# 오프라인 분석(입력: .mid 또는 .mid.lz4)
./rakobra /path/to/run00042.mid.lz4
```

실제 오프라인 분석은 보통 `ana.sh`로 런 범위를 지정해 일괄 처리합니다(예: rawdata → root/phys 또는 root/comm 으로 정리).

```bash
# 사용법:
#   ./ana.sh [comm|phys] (start_run) (end_run)
./ana.sh phys 251 254
./ana.sh comm 1 10
```

`ana.sh`는 기본적으로 아래 입력 경로를 가정합니다.

- phys: `./rawdata/KO_24_30/run{RUN}.mid.lz4`
- comm: `./rawdata/KO_comm/run{RUN}.mid.lz4`

그리고 생성된 `output000{RUN}.root`를 각각 `./root/phys`, `./root/comm`로 이동합니다.

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
python3 scripts/scaler.py <isotope_name>
# 예시: 18O 데이터의 경우 (misc/scaler_ext_18o.csv 파일이 존재해야 함)
python3 scripts/scaler.py 18o
```

스크립트는 `misc/scaler_ext_{isotope}.csv`를 읽어 `Run Summary`를 출력합니다. 코드 내 주석을 해제하면 특정 런의 `ppac_rate` 플롯, `aoq_reft.csv` 오버레이 등을 그릴 수 있습니다.

### 5.4. `rakobra_server`: 온라인 모니터링

```bash
./rakobra_server
```

서버가 뜬 뒤 브라우저에서 `http://localhost:9091`로 접속합니다(기본 페이지: `online.htm`).

### 5.5. `scripts/plot_lebt_beamdump.py`: LEBT와 BeamDump를 같은 플롯에 표시

`scripts/beam_data.py`(LEBT)와 `scripts/control_data.py`(BeamDump)를 **같은 시간축**으로 겹쳐서 그립니다(좌/우 y축 사용).

```bash
# (리포지토리 루트에서 실행 권장: misc/*.pkl 상대경로를 사용합니다)
python scripts/plot_lebt_beamdump.py \
  --date-start 2024-08-13 --time-start 13:05:00 --time-end 13:50:00 \
  --beamdump-col 9 \
  --out misc/lebt_beamdump_2024-08-13.png
```

여러 날짜를 보고 싶으면 `--date-end`(날짜 범위) 또는 `--dates`(날짜 리스트)를 지정하세요. 이 경우 **한 장의 그림에 날짜별 서브플롯으로 자동 분할**해서 그립니다.

```bash
# 날짜 범위 (서브플롯 1장으로 저장)
python scripts/plot_lebt_beamdump.py \
  --date-start 2024-08-13 --date-end 2024-08-15 \
  --time-start 13:05:00 --time-end 13:50:00 \
  --ncols 3 \
  --out misc/lebt_beamdump_2024-08-13_to_2024-08-15.png

# 날짜 리스트 (콤마 구분)
python scripts/plot_lebt_beamdump.py \
  --dates 2024-08-13,2024-08-15,2024-08-18 \
  --time-start 13:05:00 --time-end 13:50:00 \
  --ncols 3 \
  --out misc/lebt_beamdump_multi.png
```

BeamDump 컬럼이 9번이 아닐 경우, 다음으로 컬럼 인덱스/이름을 확인한 뒤 `--beamdump-col`에 숫자(인덱스) 또는 문자열(컬럼명)을 넣어주세요.

```bash
python scripts/plot_lebt_beamdump.py --date-start 2024-08-13 --time-start 13:05:00 --time-end 13:50:00 --list-control-columns
```

### 5.4. `scripts/plot_lebt_beamdump.py`: LEBT와 BeamDump를 같은 플롯에 표시

`scripts/beam_data.py`(LEBT)와 `scripts/control_data.py`(BeamDump)를 **같은 시간축**으로 겹쳐서 그립니다(좌/우 y축 사용).

```bash
# (리포지토리 루트에서 실행 권장: misc/*.pkl 상대경로를 사용합니다)
python scripts/plot_lebt_beamdump.py \
  --date-start 2024-08-13 --time-start 13:05:00 --time-end 13:50:00 \
  --beamdump-col 9 \
  --out misc/lebt_beamdump_2024-08-13.png
```

여러 날짜를 보고 싶으면 `--date-end`(날짜 범위) 또는 `--dates`(날짜 리스트)를 지정하세요. 이 경우 **한 장의 그림에 날짜별 서브플롯으로 자동 분할**해서 그립니다.

```bash
# 날짜 범위 (서브플롯 1장으로 저장)
python scripts/plot_lebt_beamdump.py \
  --date-start 2024-08-13 --date-end 2024-08-15 \
  --time-start 13:05:00 --time-end 13:50:00 \
  --ncols 3 \
  --out misc/lebt_beamdump_2024-08-13_to_2024-08-15.png

# 날짜 리스트 (콤마 구분)
python scripts/plot_lebt_beamdump.py \
  --dates 2024-08-13,2024-08-15,2024-08-18 \
  --time-start 13:05:00 --time-end 13:50:00 \
  --ncols 3 \
  --out misc/lebt_beamdump_multi.png
```

BeamDump 컬럼이 9번이 아닐 경우, 다음으로 컬럼 인덱스/이름을 확인한 뒤 `--beamdump-col`에 숫자(인덱스) 또는 문자열(컬럼명)을 넣어주세요.

```bash
python scripts/plot_lebt_beamdump.py --date-start 2024-08-13 --time-start 13:05:00 --time-end 13:50:00 --list-control-columns
```

## 6. 데이터 파일

- **KoBRA 원시 데이터:** `.mid` 형식 (MIDAS 표준)
- **분석용 ROOT 파일:** `rakobra`를 통해 생성되며, 기본 출력명은 `output000XXXXX.root`. `TTree` 형태의 이벤트 데이터 및 `TScalerData` 등이 포함될 수 있음.
- **스케일러 데이터:** `misc/scaler_ext_{isotope}.csv` 형식. `run, ts, ppac, f2pl, f3pl, hwtriga, hwtrigb, refp, reft` 등의 컬럼을 가짐.
- **A/Q vs reft 데이터:** `misc/aoq_reft.csv` (예시)
- **PID 게이트 파일:** `cut/` 디렉토리 내 텍스트 파일. 각 파일은 특정 동위원소에 대한 A/Q, Z 좌표를 정의.

## 7. 기여하기

버그를 발견하거나 개선 사항이 있다면 언제든지 이슈를 등록하거나 Pull Request를 보내주세요.

## 8. 라이선스

(프로젝트에 적용된 라이선스를 명시하세요. 예: MIT License, GPL 등)
