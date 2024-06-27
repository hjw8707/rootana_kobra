cmake_minimum_required(VERSION 3.8)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

include(${ROOT_USE_FILE})

link_directories(../ $ENV{MIDASSYS}/build)

set(SOURCES
    ana.cxx
    TAnaManager.cxx
    TTreeManager.cxx
    TDetectorManager.cxx
    TKoBRATDCHistogram.cxx
    TKoBRAADCHistogram.cxx 
    TPPACAna.cxx 
    TLPPACAna.cxx 
    TSSDAna.cxx 
    TKoBRADIGData.cxx 
    TKoBRATDCData.cxx 
    TKoBRAADCData.cxx 
    TPPACData.cxx 
    TLPPACData.cxx 
    TPlasticData.cxx 
    TSSDData.cxx 
    TDPlasticAna.cxx 
    TPlasticAna.cxx 
    KoBRA_Dict.cxx
)

set(DICTHDRS
    TPPACData.hxx 
    TLPPACData.hxx 
    TPlasticData.hxx 
    TSSDData.hxx 
)

root_generate_dictionary(KoBRA_Dict ${DICTHDRS} LINKDEF LinkDef.h)
add_executable(ana ${SOURCES})
add_library(anadict SHARED KoBRA_Dict.cxx)

target_link_libraries(ana rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)
target_include_directories(ana PUBLIC ./ ../libAnalyzer ../midasio ../mvodb ../libMidasInterface ../libAnalyzerDisplay)

