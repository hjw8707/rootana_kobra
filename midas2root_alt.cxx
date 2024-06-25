// Example Program for converting MIDAS format to ROOT format.
//
// T. Lindner (Feb 2021) 
//
// Example for creating a ROOT file for MIDAS bank without dedicated decoder.

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "TRootanaEventLoop.hxx"
#include "TFile.h"
#include "TTree.h"

#include "TAnaManager.hxx"

class Analyzer: public TRootanaEventLoop {

public:

  // An analysis manager.  Define and fill histograms in 
  // analysis manager.
  TAnaManager *anaManager;

  // The tree to fill.
  TTree *fTree;

  // save the current and voltage readings stored in this bank.
  int timestamp;
  float current_readings[9];  
  float voltage_readings[9];

  Analyzer() {

    UseBatchMode();
  };

  virtual ~Analyzer() {};

  void Initialize(){


  }
  
  
  void BeginRun(int transition,int run,int time){
    
    // Create a TTree
    fTree = new TTree("midas_data","MIDAS data");

    fTree->Branch("timestamp",&timestamp,"timestamp/I");
    fTree->Branch("current_readings",current_readings,"current_readings[9]/F");
    fTree->Branch("voltage_readings",voltage_readings,"voltage_readings[9]/F");

  }   


  void EndRun(int transition,int run,int time){
        printf("\n");
  }

  
  
  // Main work here; create ttree events for every sequenced event in 
  // Lecroy data packets.
  bool ProcessMidasEvent(TDataContainer& dataContainer){

    timestamp = dataContainer.GetMidasEvent().GetTimeStamp();
 
    // Use generic data class to get the data bank.
    // You need to know that the data is stored as a float.
    // This particular bank is 18 floats, which pairs of float as current and voltage readings.
    TGenericData *data = dataContainer.GetEventData<TGenericData>("BRV1");
    if(data){
      std::cout << "Found BRV1 bank" << std::endl;
      std::cout << "Bank size: " << data->GetSize() << std::endl;
      for(int i = 0; i < data->GetSize() ; i++){
	std::cout << "Data["<<i<<"] is " << data->GetFloat()[i] << std::endl;
	int index = i/2;
	if(i%2==0){
	  current_readings[index] = data->GetFloat()[i];
	}else{
	  voltage_readings[index] = data->GetFloat()[i];
	}
      }

      fTree->Fill();

    }


    return true;

  };
  


  
}; 


int main(int argc, char *argv[])
{

  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);

}

