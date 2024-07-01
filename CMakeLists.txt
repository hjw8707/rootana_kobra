cmake_minimum_required(VERSION 3.8)
project(kobra_ana)

find_package(ROOT COMPONENTS Gui RHTTP XMLIO XMLParser)

include(${ROOT_USE_FILE})

link_directories(./ ../ $ENV{MIDASSYS}/build)

set(HAVE_ROOT_HTTP 1)
set(HAVE_THTTP_SERVER 1)

set(SOURCES
    TAnaManager.cxx
    TTreeManager.cxx
    TDetectorManager.cxx
    TCanvasManager.cxx
    THistManager.cxx
    TKoBRASniffer.cxx
    TKoBRATDCHistogram.cxx
    TKoBRAADCHistogram.cxx 
    TKoBRADIGHistogram.cxx
    TPPACHistogram.cxx
    TPlasticHistogram.cxx
    TSSDHistogram.cxx
    TPIDHistogram.cxx    
    TTrackHistogram.cxx
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
    TKoBRASniffer.hxx    
)

root_generate_dictionary(KoBRA_Dict ${DICTHDRS} LINKDEF LinkDef.h)
#add_executable(ana ana.cxx ${SOURCES})
#add_executable(anaDisplay anaDisplay.cxx ${SOURCES})
#add_executable(root_server root_server.cxx ${SOURCES})

add_library(kobra SHARED KoBRA_Dict.cxx ${SOURCES})
target_include_directories(kobra PUBLIC ./ ../libAnalyzerDisplay ../libAnalyzer ../midasio ../mvodb ../libMidasInterface )

add_executable(ana ana.cxx)
target_link_libraries(ana kobra rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)

add_executable(root_server root_server.cxx)
target_link_libraries(root_server kobra rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)

#target_link_libraries(anaDisplay rootana -lm -lz -lpthread -lssl -lmidas ${ROOT_LIBRARIES} -lThread -lrt -lutil)
#target_include_directories(anaDisplay PUBLIC ./ ../libAnalyzer ../midasio ../mvodb ../libMidasInterface ../libAnalyzerDisplay)


#target_include_directories(root_server PUBLIC ./ ../libAnalyzer ../midasio ../mvodb ../libMidasInterface ../libAnalyzerDisplay)
