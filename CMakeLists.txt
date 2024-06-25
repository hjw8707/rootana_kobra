cmake_minimum_required(VERSION 3.8)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

include(${ROOT_USE_FILE})

link_directories(../ $ENV{MIDASSYS}/build)

root_generate_dictionary(KoBRA_Dict TPPACData.hxx TPlasticData.hxx LINKDEF LinkDef.h)

add_executable(ana ana.cxx TAnaManager.cxx TKoBRATDCHistogram.cxx TKoBRAADCHistogram.cxx 
TPPACAna.cxx TKoBRATDCData.cxx TKoBRAADCData.cxx TPPACData.cxx TPlasticData.cxx TPlasticAna.cxx KoBRA_Dict.cxx)

target_link_libraries(ana rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)
target_include_directories(ana PUBLIC ./ ../libAnalyzer ../midasio ../mvodb ../libMidasInterface ../libAnalyzerDisplay)

