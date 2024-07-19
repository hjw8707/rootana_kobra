#include "TAnaManager.hxx"
#include "TV1190Data.hxx"
#include "TRB3Decoder.hxx"

#include "TTreeManager.hxx"
#include "TTreeFormula.h"

#include "TH1.h"
#include <string>

TAnaManager::TAnaManager()
{
  treeMan = TTreeManager::GetInstance();
  treeMan->MakeTree("kobra", "kobra");
  treeMan->MakeHeader();
  treeMan->LoadAlias("aliaslist.txt");
  detMan = TDetectorManager::GetInstance();
  detMan->LoadAnaList("analist.txt");
  detMan->PrintParametersAll();
  detMan->SetTreeAll();

  // AddHistogram(new TPlasticHistograms("f2dpla", detMan->GetDetectorAna("f2dpla")->GetDataArray()));
  // AddHistogram(new TPlasticHistograms("f3dpla", detMan->GetDetectorAna("f3dpla")->GetDataArray()));
  /*
    AddHistogram(new TKoBRATDCHistograms("F1PA"));
    AddHistogram(new TKoBRATDCHistograms("F2PA"));
    AddHistogram(new TKoBRATDCHistograms("F3PA"));
    AddHistogram(new TKoBRAADCHistograms("F3SI"));
    AddHistogram(new TKoBRADIGHistograms("V1730"));
    AddHistogram(new TKoBRADIGHistograms("V221730"));
        AddHistogram(new TPPACHistograms("f1uppac", detMan->GetDetectorAna("f1uppac")->GetDataArray()));
        AddHistogram(new TPPACHistograms("f1dppac", detMan->GetDetectorAna("f1dppac")->GetDataArray()));
        AddHistogram(new TPPACHistograms("f2uppac", detMan->GetDetectorAna("f2uppac")->GetDataArray()));
        AddHistogram(new TPPACHistograms("f2dppac", detMan->GetDetectorAna("f2dppac")->GetDataArray()));
        AddHistogram(new TPPACHistograms("f3uppac", detMan->GetDetectorAna("f3uppac")->GetDataArray()));
        AddHistogram(new TPPACHistograms("f3dppac", detMan->GetDetectorAna("f3dppac")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f2pla", detMan->GetDetectorAna("f2pla")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f3pla", detMan->GetDetectorAna("f3pla")->GetDataArray()));

        AddHistogram(new TPlasticHistograms("f1ppt", detMan->GetDetectorAna("f1ppt")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f2uppt", detMan->GetDetectorAna("f2uppt")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f2dppt", detMan->GetDetectorAna("f2dppt")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f3uppt", detMan->GetDetectorAna("f3uppt")->GetDataArray()));
        AddHistogram(new TPlasticHistograms("f3dppt", detMan->GetDetectorAna("f3dppt")->GetDataArray()));
        AddHistogram(new TSSDHistograms("f2ssd", detMan->GetDetectorAna("f2ssd")->GetDataArray()));
        AddHistogram(new TSSDHistograms("f3ssd", detMan->GetDetectorAna("f3ssd")->GetDataArray()));
        AddHistogram(new TPIDHistograms("pid", detMan->GetDetectorAna("f2pla")->GetDataArray(),
                                        detMan->GetDetectorAna("f3pla")->GetDataArray(),
                                        detMan->GetDetectorAna("f2ssd")->GetDataArray(),
                                        detMan->GetDetectorAna("f3ssd")->GetDataArray()));

      */
  histMan = THistManager::GetInstance();
  histMan->AddHistFromFile("histlist.txt");
  canMan = TCanvasManager::GetInstance();
  canMan->AddCanvasAndHistFromFile("canlist.txt");
};

void TAnaManager::BeginRun(int transition, int run, int time)
{
  std::cout << "Start Run " << run << ", " << time << std::endl;
  std::cout << "Transition " << transition << std::endl;
  treeMan->GetHeader()->SetRunNum(run);
  treeMan->GetHeader()->SetStartTimeStamp(time);
  // histMan->Reset();
  // treeMan->Reset();
}

void TAnaManager::EndRun(int, int run, int time)
{
  treeMan->GetHeader()->SetStopTimeStamp(time);
  treeMan->WriteHeader();
  std::cout << "End Run " << run << ", " << time << std::endl;
}

void TAnaManager::AddHistogram(THistogramArrayBase *histo)
{
  histo->DisableAutoUpdate();

  // histo->CreateHistograms();
  fHistos.push_back(histo);
}

int TAnaManager::ProcessMidasEvent(TDataContainer &dataContainer)
{

  // Fill all the  histograms
  for (unsigned int i = 0; i < fHistos.size(); i++)
  {
    // Some histograms are very time-consuming to draw, so we can
    // delay their update until the canvas is actually drawn.
    if (!fHistos[i]->IsUpdateWhenPlotted())
    {
      fHistos[i]->UpdateHistograms(dataContainer);
    }
  }

  detMan->ClearAll();
  detMan->SetDataAll(dataContainer);
  detMan->AnalysisAll();

  treeMan->Fill();
  treeMan->GetLastEntry(); // necessary for TTreeFormula
  histMan->Fill();
  return 1;
}

// Little trick; we only fill the transient histograms here (not cumulative), since we don't want
// to fill histograms for events that we are not displaying.
// It is pretty slow to fill histograms for each event.
void TAnaManager::UpdateTransientPlots(TDataContainer &dataContainer)
{
  std::vector<THistogramArrayBase *> histos = GetHistograms();

  for (unsigned int i = 0; i < histos.size(); i++)
  {
    if (histos[i]->IsUpdateWhenPlotted())
    {
      histos[i]->UpdateHistograms(dataContainer);
    }
  }
}
