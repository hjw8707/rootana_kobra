#include "TAnaManager.hxx"

#include <string>

#include "TH1.h"
#include "TRB3Decoder.hxx"
#include "TTreeFormula.h"
#include "TTreeManager.hxx"
#include "TV1190Data.hxx"

TAnaManager::TAnaManager() {
  server = nullptr;

  treeMan = TTreeManager::GetInstance();
  treeMan->MakeTree("kobra", "kobra");
  treeMan->MakeHeader();
  treeMan->LoadAlias("aliaslist.txt");
  detMan = TDetectorManager::GetInstance();
  detMan->LoadAnaList("analist.txt");
  detMan->PrintParametersAll();
  detMan->SetTreeAll();

  histMan = THistManager::GetInstance();
  histMan->AddHistFromFile("histlist.txt");
  canMan = TCanvasManager::GetInstance();
  canMan->AddCanvasAndHistFromFile("canlist.txt");
};

void TAnaManager::BeginRun(int transition, int run, int time) {
  std::cout << "Start Run " << run << ", " << time << std::endl;
  std::cout << "Transition " << transition << std::endl;
  treeMan->GetHeader()->SetRunNum(run);
  treeMan->GetHeader()->SetStartTimeStamp(time);
  treeMan->SetRunN(run);
  // histMan->Reset();
  // treeMan->Reset();
}

void TAnaManager::EndRun(int, int run, int time) {
  treeMan->GetHeader()->SetStopTimeStamp(time);
  treeMan->WriteHeader();
  std::cout << "End Run " << run << ", " << time << std::endl;
}

void TAnaManager::AddHistogram(THistogramArrayBase *histo) {
  histo->DisableAutoUpdate();

  // histo->CreateHistograms();
  fHistos.push_back(histo);
}

int TAnaManager::ProcessMidasEvent(TDataContainer &dataContainer) {
  // Fill all the  histograms
  for (unsigned int i = 0; i < fHistos.size(); i++) {
    // Some histograms are very time-consuming to draw, so we can
    // delay their update until the canvas is actually drawn.
    if (!fHistos[i]->IsUpdateWhenPlotted()) {
      fHistos[i]->UpdateHistograms(dataContainer);
    }
  }
  detMan->ClearAll();
  detMan->SetDataAll(dataContainer);
  detMan->AnalysisAll();

  treeMan->Fill();
  if (IsTHttpServer()) {
    treeMan->GetLastEntry();  // necessary for TTreeFormula
    histMan->Fill();
  }
  return 1;
}

// Little trick; we only fill the transient histograms here (not cumulative), since we don't want
// to fill histograms for events that we are not displaying.
// It is pretty slow to fill histograms for each event.
void TAnaManager::UpdateTransientPlots(TDataContainer &dataContainer) {
  std::vector<THistogramArrayBase *> histos = GetHistograms();

  for (unsigned int i = 0; i < histos.size(); i++) {
    if (histos[i]->IsUpdateWhenPlotted()) {
      histos[i]->UpdateHistograms(dataContainer);
    }
  }
}
