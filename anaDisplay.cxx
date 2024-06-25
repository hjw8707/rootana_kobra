#include <stdio.h>
#include <iostream>

#include "TRootanaDisplay.hxx"
#include "TH1D.h"
#include "TV792Data.hxx"

#include "TFancyHistogramCanvas.hxx"
#include "TInterestingEventManager.hxx"


#include "TAnaManager.hxx"
#include "TTRB3Data.hxx"

class MyTestLoop: public TRootanaDisplay { 

public:
	
  // An analysis manager.  Define and fill histograms in 
  // analysis manager.
  TAnaManager *anaManager;

  MyTestLoop() {
    SetOutputFilename("example_output");
    DisableRootOutput(false);
    anaManager = new TAnaManager();
    // Number of events to skip before plotting one.
    //SetNumberSkipEvent(10);
    // Choose to use functionality to update after X seconds
    SetOnlineUpdatingBasedSeconds();
    // Uncomment this to enable the 'interesting event' functionality.
    //iem_t::instance()->Enable();
  }

  void AddAllCanvases(){

    // Set up tabbed canvases

    // Set up all the listed canvases from the AnaManager list of THistogramArrayBases
    std::vector<THistogramArrayBase*> histos = anaManager->GetHistograms();
    
    for (unsigned int i = 0; i < histos.size(); i++) {
      TCanvasHandleBase* canvas = histos[i]->CreateCanvas();
      if (canvas) {
        AddSingleCanvas(canvas, histos[i]->GetTabName());
      }
    }
        
    SetDisplayName("Example Display");
  };

  virtual ~MyTestLoop() {};

  void BeginRun(int transition,int run,int time) {
    std::cout << "User BOR method" << std::endl;
    anaManager->BeginRun(transition, run, time);
    std::cout << "End User BOR method" << std::endl;
  }

  void EndRun(int transition,int run,int time) {
    std::cout << "User EOR method" << std::endl;
    anaManager->EndRun(transition, run, time);
  }

  void ResetHistograms(){}

  void UpdateHistograms(TDataContainer& dataContainer){
    // Update the cumulative histograms here
    anaManager->ProcessMidasEvent(dataContainer);
  }

  void PlotCanvas(TDataContainer& dataContainer){
    // Update the transient (per-event) histograms here.
    // saves CPU to not update them always when not being used.
    anaManager->UpdateTransientPlots(dataContainer);
  }
  
}; 






int main(int argc, char *argv[])
{
  MyTestLoop::CreateSingleton<MyTestLoop>();  
  return MyTestLoop::Get().ExecuteLoop(argc, argv);
}

