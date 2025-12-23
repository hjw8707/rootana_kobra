# CMake의 최소 버전 지정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 설정
project(kobra_ana)

# ROOT 패키지의 필요한 컴포넌트 찾기
find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

# CMake 모듈 경로에 ROOT의 cmake 모듈 추가
list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
include(${ROOT_USE_FILE})

# include 디렉토리 및 외부 라이브러리 include 경로 추가
include_directories(./include $ENV{ROOTANASYS}/include $ENV{MIDASSYS}/include ./macros)
# 라이브러리 디렉토리 추가
link_directories(./ $ENV{ROOTANASYS}/lib $ENV{MIDASSYS}/lib)

# ROOT HTTP 관련 옵션 설정
set(HAVE_ROOT_HTTP 1)
set(HAVE_THTTP_SERVER 1)

# 소스 파일 목록 생성 (src/*.cxx)
file(GLOB SOURCES "src/*.cxx")
# 추가 소스 파일 명시적으로 추가
list(APPEND SOURCES "macros/kobra.cxx" "macros/kobra_scaler.cxx" "macros/kobra_beam.cxx" "macros/kobra_f0.cxx")
list(APPEND SOURCES KoBRA_Dict.cxx)

# include 경로 변수 설정 (INC)
set(INC
    ./                # 현재 디렉토리
    ./include         # include 디렉토리
    ./macros          # macros 디렉토리
    $ENV{ROOTANASYS}/include # 외부 ROOTANA include
)

# dictionary 생성을 위한 헤더 파일 목록
set(DICTHDRS
  "include/THeader.hxx"
  "include/TPPACData.hxx"
  "include/TPPACRawData.hxx"
  "include/TLPPACData.hxx"
  "include/TLPPACRawData.hxx"
  "include/TPlasticData.hxx"
  "include/TSSDData.hxx"
  "include/TScalerData.hxx"
  "include/TKoBRASniffer.hxx"
  "macros/kobra.hxx"
  "macros/kobra_scaler.hxx"
  "macros/kobra_beam.hxx"
  "macros/kobra_f0.hxx"
  "$ENV{ROOTANASYS}/include/TRootanaDisplay.hxx"
)

# ROOT dictionary 생성 명령
root_generate_dictionary(KoBRA_Dict ${DICTHDRS}
                        LINKDEF LinkDef.h)

# kobra 라이브러리 생성 (SHARED)
add_library(kobra SHARED ${SOURCES})
# kobra 라이브러리에 include 경로 추가
target_include_directories(kobra PUBLIC ${INC})
# kobra 라이브러리에 필요한 라이브러리 링크
target_link_libraries(kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil -lcling)

# rakobra 실행 파일 생성 및 라이브러리 링크
add_executable(rakobra rakobra.cxx)
target_link_libraries(rakobra kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)

# rakobra_server 실행 파일 생성 및 라이브러리 링크
add_executable(rakobra_server rakobra_server.cxx)
target_link_libraries(rakobra_server kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)

# midcheck 실행 파일 생성 및 라이브러리 링크
add_executable(midcheck midcheck.cxx)
target_link_libraries(midcheck kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)
