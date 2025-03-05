cmake_minimum_required(VERSION 3.10...3.20)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
include(${ROOT_USE_FILE})

link_directories(./ $ENV{ROOTANASYS}/lib $ENV{MIDASSYS}/lib)

set(HAVE_ROOT_HTTP 1)
set(HAVE_THTTP_SERVER 1)

file(GLOB SOURCES "src/*.cxx")
#list(APPEND SOURCES macros/kobra.cxx)
list(APPEND SOURCES KoBRA_Dict.cxx)

set(INC
./
./include
./macros
$ENV{ROOTANASYS}/include
)

set(DICTHDRS
  "include/THeader.hxx"
  "include/TPPACData.hxx"
    "include/TLPPACData.hxx"
    "include/TPlasticData.hxx"
    "include/TSSDData.hxx"
    "include/TScalerData.hxx"
    "include/TKoBRASniffer.hxx"
)

set(DICTSRCSONLY
    "src/TPPACData.cxx"
    "src/TLPPACData.cxx"
    "src/TPlasticData.cxx"
    "src/TSSDData.cxx"
    "src/TScalerData.cxx"
    "macros/kobra.cxx"
    "macros/curry.cxx"
)

set(DICTHDRSONLY
  "include/THeader.hxx"
  "include/TPPACData.hxx"
    "include/TLPPACData.hxx"
    "include/TPlasticData.hxx"
    "include/TSSDData.hxx"
    "include/TScalerData.hxx"
    "macros/kobra.hxx"
    "macros/curry.hxx"
)

root_generate_dictionary(KoBRA_Dict ${DICTHDRS} LINKDEF LinkDef.h)
root_generate_dictionary(KoBRA_DictOnly ${DICTHDRSONLY} LINKDEF LinkDefOnly.h)

add_library(anadict SHARED KoBRA_DictOnly.cxx ${DICTSRCSONLY})
target_include_directories(anadict PUBLIC ./ ./include ./macros )
target_link_libraries(anadict ${ROOT_LIBRARIES})
add_library(kobra SHARED ${SOURCES})
target_include_directories(kobra PUBLIC ${INC})
target_link_libraries(kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES})

add_executable(rakobra rakobra.cxx)
target_link_libraries(rakobra kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)

add_executable(rakobra_server rakobra_server.cxx)
target_link_libraries(rakobra_server kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)

add_executable(midcheck midcheck.cxx)
target_link_libraries(midcheck kobra rootana -lm -lz -lpthread -lmidas ${ROOT_LIBRARIES} -lThread -lutil)
