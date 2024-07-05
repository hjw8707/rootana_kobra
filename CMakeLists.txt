cmake_minimum_required(VERSION 3.8)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

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
    "include/TPPACData.hxx"
    "include/TLPPACData.hxx"
    "include/TPlasticData.hxx" 
    "include/TSSDData.hxx" 
    "include/TKoBRASniffer.hxx"
)

set(DICTSRCSONLY
    "src/TPPACData.cxx"
    "src/TLPPACData.cxx"
    "src/TPlasticData.cxx"
    "src/TSSDData.cxx"
    "macros/kobra.cxx"
)

set(DICTHDRSONLY
    "include/TPPACData.hxx"
    "include/TLPPACData.hxx"
    "include/TPlasticData.hxx"
    "include/TSSDData.hxx"
    "macros/kobra.hxx"
)

root_generate_dictionary(KoBRA_Dict ${DICTHDRS} LINKDEF LinkDef.h)
root_generate_dictionary(KoBRA_DictOnly ${DICTHDRSONLY} LINKDEF LinkDefOnly.h)

add_library(anadict SHARED KoBRA_DictOnly.cxx ${DICTSRCSONLY})
target_include_directories(anadict PUBLIC ./ ./include ./macros )
add_library(kobra SHARED KoBRA_Dict.cxx ${SOURCES})
target_include_directories(kobra PUBLIC ${INC})

add_executable(rakobra rakobra.cxx)
target_link_libraries(rakobra kobra rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)

add_executable(rakobra_server rakobra_server.cxx)
target_link_libraries(rakobra_server kobra rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)
