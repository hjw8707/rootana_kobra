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
  ppac_f1u = new TLPPACAna("F1_U","PAR_F1UPPAC.txt");
  ppac_f1d = new TLPPACAna("F1_D","PAR_F1DPPAC.txt");  
  ppac_f1u->PrintParameters();
  ppac_f1u->SetTree();
  ppac_f1d->PrintParameters();
  ppac_f1d->SetTree();

  ppac_f2u = new TPPACAna("F2_U","PAR_F2UPPAC.txt");
  ppac_f2d = new TPPACAna("F2_D","PAR_F2DPPAC.txt");  
  ppac_f2u->PrintParameters();
  ppac_f2u->SetTree();
  ppac_f2d->PrintParameters();
  ppac_f2d->SetTree();

  ppac_f3u = new TPPACAna("F3_U","PAR_F3UPPAC.txt");
  ppac_f3d = new TPPACAna("F3_D","PAR_F3DPPAC.txt");  
  ppac_f3u->PrintParameters();
  ppac_f3u->SetTree();
  ppac_f3d->PrintParameters();
  ppac_f3d->SetTree();  

  pla_f2 = new TPlasticAna("F2","PAR_F2Plastic.txt");
  pla_f2->PrintParameters();
  pla_f2->SetTree();
  pla_f3 = new TPlasticAna("F3","PAR_F3Plastic.txt");
  pla_f3->PrintParameters();
  pla_f3->SetTree();  

  ssd_f2 = new TSSDAna("F2","PAR_F2SSD.txt");
  ssd_f2->PrintParameters();
  ssd_f2->SetTree();
  ssd_f3 = new TSSDAna("F3","PAR_F3SSD.txt");
  ssd_f3->PrintParameters();
  ssd_f3->SetTree();    
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
  

  ////////////////////////////////////////////////////////////////////////
  // PPAC analysis
  ppac_f1u->Clear();
  ppac_f1u->SetData(dataContainer.GetEventData<TKoBRATDCData>("F1PA")
                ->GetMeasurements());
  ppac_f1u->Analysis();

  ppac_f1d->Clear();
  ppac_f1d->SetData(dataContainer.GetEventData<TKoBRATDCData>("F1PA")
                ->GetMeasurements());
  ppac_f1d->Analysis();  
  //ppac_f1u->PrintOutdata();

  ppac_f2u->Clear();
  ppac_f2u->SetData(dataContainer.GetEventData<TKoBRATDCData>("F2PA")
                ->GetMeasurements());
  ppac_f2u->Analysis();
  //ppac_f2u->PrintOutdata();

  ppac_f2d->Clear();
  ppac_f2d->SetData(dataContainer.GetEventData<TKoBRATDCData>("F2PA")
                ->GetMeasurements());
  ppac_f2d->Analysis();
  //ppac_f2d->PrintOutdata();

  ppac_f3u->Clear();
  ppac_f3u->SetData(dataContainer.GetEventData<TKoBRATDCData>("F3PA")
                ->GetMeasurements());
  ppac_f3u->Analysis();
  //ppac_f3u->PrintOutdata();

  ppac_f3d->Clear();
  ppac_f3d->SetData(dataContainer.GetEventData<TKoBRATDCData>("F3PA")
                ->GetMeasurements());
  ppac_f3d->Analysis();
  //ppac_f3d->PrintOutdata();  
  ////////////////////////////////////////////////////////////////////////  

  ////////////////////////////////////////////////////////////////////////
  // Plastic analysis
  pla_f2->Clear();
  pla_f2->SetData(dataContainer.GetEventData<TKoBRATDCData>("F2PA")
                ->GetMeasurements());
  pla_f2->Analysis();
  pla_f3->Clear();
  pla_f3->SetData(dataContainer.GetEventData<TKoBRATDCData>("F3PA")
                ->GetMeasurements());
  pla_f3->Analysis();  
  //pla_f2->PrintOutdata();
  ////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////
  // SSD analysis
  ssd_f2->Clear();
  ssd_f2->SetData(dataContainer.GetEventData<TKoBRAADCData>("F3SI")
                ->GetMeasurements());
  ssd_f2->Analysis();
  ssd_f3->Clear();
  ssd_f3->SetData(dataContainer.GetEventData<TKoBRAADCData>("F3SI")
                ->GetMeasurements());
  ssd_f3->Analysis();  
  //ssd_f2->PrintOutdata();
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


