#include "TAnaManager.hxx"
#include "TV1190Data.hxx"
#include "TRB3Decoder.hxx"

TAnaManager::TAnaManager(){

  AddHistogram(new TKoBRATDCHistograms("F1PA"));
  AddHistogram(new TKoBRATDCHistograms("F2PA"));
  AddHistogram(new TKoBRATDCHistograms("F3PA"));
  AddHistogram(new TKoBRAADCHistograms("F3SI"));
      
};

void TAnaManager::BeginRun(int, int, int) {  
  ppac_f2u = new TPPACAna("F2_U","PAR_F2UPPAC.txt");
  ppac_f2d = new TPPACAna("F2_D","PAR_F2DPPAC.txt");  
  ppac_f2u->PrintParameters();
  ppac_f2d->PrintParameters();  
}

void TAnaManager::EndRun(int, int, int) {
}

void TAnaManager::AddHistogram(THistogramArrayBase* histo) {
  histo->DisableAutoUpdate();

  //histo->CreateHistograms();
  fHistos.push_back(histo);
}


int TAnaManager::ProcessMidasEvent(TDataContainer& dataContainer){


  // Fill all the  histograms
  for (unsigned int i = 0; i < fHistos.size(); i++) {
    // Some histograms are very time-consuming to draw, so we can
    // delay their update until the canvas is actually drawn.
    if (!fHistos[i]->IsUpdateWhenPlotted()) {
      fHistos[i]->UpdateHistograms(dataContainer);
    }
  }
  
  if(0){

    TKoBRATDCData* adc = dataContainer.GetEventData<TKoBRATDCData>("F2PA");
    if(adc) {
      std::cout << adc->GetMeasurements().size() << std::endl;
      adc->Print();
    }

  }

  ////////////////////////////////////////////////////////////////////////
  // PPAC analysis
  ppac_f2u->Clear();
  ppac_f2u->SetData(dataContainer.GetEventData<TKoBRATDCData>("F2PA")
                ->GetMeasurements());
  ppac_f2u->Analysis();
  ppac_f2u->PrintOutdata();

  ppac_f2d->Clear();
  ppac_f2d->SetData(dataContainer.GetEventData<TKoBRATDCData>("F2PA")
                ->GetMeasurements());
  ppac_f2d->Analysis();
  ppac_f2d->PrintOutdata();
  ////////////////////////////////////////////////////////////////////////  

  return 1;
}


// Little trick; we only fill the transient histograms here (not cumulative), since we don't want
// to fill histograms for events that we are not displaying.
// It is pretty slow to fill histograms for each event.
void TAnaManager::UpdateTransientPlots(TDataContainer& dataContainer){
  std::vector<THistogramArrayBase*> histos = GetHistograms();
  
  for (unsigned int i = 0; i < histos.size(); i++) {
    if (histos[i]->IsUpdateWhenPlotted()) {
      histos[i]->UpdateHistograms(dataContainer);
    }
  }
}


