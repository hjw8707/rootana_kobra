#include "TKoBRATDCHistogram.h"

#include "TKoBRATDCData.hxx"
#include "TDirectory.h"
#include "TH2.h"

const int Nchannels = 32;

/// Reset the histograms for this canvas
TKoBRATDCHistograms::TKoBRATDCHistograms(const char* _bankname){  
  bankname = _bankname;
  SetSubTabName(_bankname);
  CreateHistograms();
}


void TKoBRATDCHistograms::CreateHistograms(){
  // Otherwise make histograms
  clear();
  
  std::cout << "Create Histos" << std::endl;
  for(int i = 0; i < Nchannels; i++){ // loop over channels    

    char name[100];
    char title[100];
    sprintf(name,"%s_%i",bankname.c_str(),i);

    // Delete old histograms, if we already have them
    TH1D *old = (TH1D*)gDirectory->Get(name);
    if (old) delete old;

    // Create new histograms
    sprintf(title,"%s histogram for channel=%i",bankname.c_str(),i);	
    
    TH1D *tmp = new TH1D(name,title,5000,0,500000);
    tmp->SetXTitle("TDC value");
    tmp->SetYTitle("Number of Entries");
    push_back(tmp);
  }

  char name[100];
  char title[100];
  sprintf(name,"%s_all",bankname.c_str());
  TH2D *old = (TH2D*)gDirectory->Get(name);
  if (old) delete old;
  sprintf(title,"%s histogram for all",bankname.c_str());	
  TH2D *tmp = new TH2D(name,title,Nchannels,0,Nchannels,5000,0,500000);
  tmp->SetXTitle("Channel");
  tmp->SetYTitle("TDC value");
  push_back(tmp);

}

/// Update the histograms for this canvas.
void TKoBRATDCHistograms::UpdateHistograms(TDataContainer& dataContainer){
  TKoBRATDCData *data = dataContainer.GetEventData<TKoBRATDCData>(bankname.c_str());
  if(!data) return;

  /// Get the Vector of ADC Measurements.
  std::vector<KoBRATDCMeasurement> measurements = data->GetMeasurements();
  for(unsigned int i = 0; i < measurements.size(); i++){ // loop over measurements
    int chan = measurements[i].GetChannel();
    GetHistogram(chan)->Fill(measurements[i].GetMeasurement());
    GetHistogram(Nchannels)->Fill(chan,measurements[i].GetMeasurement());
  }
}

/// Take actions at begin run
void TKoBRATDCHistograms::BeginRun(int transition,int run,int time){
  CreateHistograms();
}

/// Take actions at end run  
void TKoBRATDCHistograms::EndRun(int transition,int run,int time){
}
