#include "TAnaManager.hxx"
#include "TV1190Data.hxx"
#include "TRB3Decoder.hxx"

TAnaManager::TAnaManager()
{
  detMan = TDetectorManager::GetInstance();

  AddHistogram(new TKoBRATDCHistograms("F1PA"));
  AddHistogram(new TKoBRATDCHistograms("F2PA"));
  AddHistogram(new TKoBRATDCHistograms("F3PA"));
  AddHistogram(new TKoBRAADCHistograms("F3SI"));
};

void TAnaManager::BeginRun(int, int, int)
{
  ppac_f1u = new TLPPACAna("F1_U", "PAR_F1UPPAC.txt");
  ppac_f1d = new TLPPACAna("F1_D", "PAR_F1DPPAC.txt");
  ppac_f2u = new TPPACAna("F2_U", "PAR_F2UPPAC.txt");
  ppac_f2d = new TPPACAna("F2_D", "PAR_F2DPPAC.txt");
  ppac_f3u = new TPPACAna("F3_U", "PAR_F3UPPAC.txt");
  ppac_f3d = new TPPACAna("F3_D", "PAR_F3DPPAC.txt");
  ppac_sc = new TPPACAna("SC", "PAR_SCPPAC.txt");

  pla_f2 = new TPlasticAna("F2", "PAR_F2Plastic.txt");
  pla_f3 = new TPlasticAna("F3", "PAR_F3Plastic.txt");
  dpla_f2 = new TDPlasticAna("F2", "PAR_F2DPlastic.txt");
  dpla_f3 = new TDPlasticAna("F3", "PAR_F3DPlastic.txt");

  ppt_f1 = new TPlasticAna("F1PP", "PAR_F1PPT.txt", 1);
  ppt_f2u = new TPlasticAna("F2PPU", "PAR_F2PPTU.txt", 1);
  ppt_f2d = new TPlasticAna("F2PPU", "PAR_F2PPTD.txt", 1);
  ppt_f3u = new TPlasticAna("F3PPU", "PAR_F3PPTU.txt", 1);
  ppt_f3d = new TPlasticAna("F3PPU", "PAR_F3PPTD.txt", 1);

  ssd_f2 = new TSSDAna("F2", "PAR_F2SSD.txt", 16);
  ssd_f3 = new TSSDAna("F3", "PAR_F3SSD.txt", 16);

  detMan->AddDetectorAna("f1uppac", ppac_f1u, KoBRATDC, "F1PA");
  detMan->AddDetectorAna("f1dppac", ppac_f1d, KoBRATDC, "F1PA");
  detMan->AddDetectorAna("f2uppac", ppac_f2u, KoBRATDC, "F2PA");
  detMan->AddDetectorAna("f2dppac", ppac_f2d, KoBRATDC, "F2PA");
  detMan->AddDetectorAna("f3uppac", ppac_f3u, KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f3dppac", ppac_f3d, KoBRATDC, "F3PA");
  detMan->AddDetectorAna("scppac",  ppac_sc,  KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f2pla",   pla_f2,   KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f3pla",   pla_f3,   KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f2dpla",  dpla_f2,  KoBRADIG, "V1730");
  detMan->AddDetectorAna("f3dpla",  dpla_f3,  KoBRADIG, "V1730");
  detMan->AddDetectorAna("f1ppt",   ppt_f1,   KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f2uppt",  ppt_f2u,  KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f2dppt",  ppt_f2d,  KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f3uppt",  ppt_f3u,  KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f3dppt",  ppt_f3d,  KoBRATDC, "F3PA");
  detMan->AddDetectorAna("f2ssd",   ssd_f2,   KoBRAADC, "F3SI");
  detMan->AddDetectorAna("f3ssd",   ssd_f3,   KoBRAADC, "F3SI");

  detMan->PrintParametersAll();
  detMan->SetTreeAll();
}

void TAnaManager::EndRun(int, int, int)
{
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
