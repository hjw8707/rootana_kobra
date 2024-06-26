cmake_minimum_required(VERSION 3.8)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

include(${ROOT_USE_FILE})

link_directories(../ $ENV{MIDASSYS}/build)

root_generate_dictionary(KoBRA_Dict TPPACData.hxx TLPPACData.hxx TPlasticData.hxx TSSDData.hxx LINKDEF LinkDef.h)

add_executable(ana ana.cxx TAnaManager.cxx TTreeManager.cxx TKoBRATDCHistogram.cxx TKoBRAADCHistogram.cxx 
TPPACAna.cxx TLPPACAna.cxx TSSDAna.cxx TKoBRATDCData.cxx TKoBRAADCData.cxx TPPACData.cxx TLPPACData.cxx TPlasticData.cxx TSSDData.cxx TPlasticAna.cxx KoBRA_Dict.cxx)

add_library(anadict SHARED KoBRA_Dict.cxx)

target_link_libraries(ana rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)
target_include_directories(ana PUBLIC ./ ../libAnalyzer ../midasio ../mvodb ../libMidasInterface ../libAnalyzerDisplay)

