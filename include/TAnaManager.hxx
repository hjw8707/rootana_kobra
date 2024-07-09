#ifndef TAnaManager_h
#define TAnaManager_h

#include "TTree.h"

#include "TKoBRADIGData.hxx"
#include "TKoBRAADCData.hxx"
#include "TKoBRATDCData.hxx"
#include "TKoBRATDCHistogram.hxx"
#include "TKoBRAADCHistogram.hxx"
#include "TKoBRADIGHistogram.hxx"
#include "TPPACHistogram.hxx"
#include "TPlasticHistogram.hxx"
#include "TSSDHistogram.hxx"
#include "TPIDHistogram.hxx"
#include "TTrackHistogram.hxx"

#include "TDetectorManager.hxx"
#include "TCanvasManager.hxx"
#include "THistManager.hxx"
#include "TTreeManager.hxx"

#include "THttpServer.h"

class TAnaManager
{
public:
  TAnaManager();
  virtual ~TAnaManager() {};

  /// Processes the midas event, fills histograms, etc.
  int ProcessMidasEvent(TDataContainer &dataContainer);

  /// Update those histograms that only need to be updated when we are plotting.
  void UpdateForPlotting();

  void Initialize();

  bool CheckOption(std::string option);

  void BeginRun(int transition, int run, int time);
  void EndRun(int transition, int run, int time);

  // Add a THistogramArrayBase object to the list
  void AddHistogram(THistogramArrayBase *histo);

  // Little trick; we only fill the transient histograms here (not cumulative), since we don't want
  // to fill histograms for events that we are not displaying.
  // It is pretty slow to fill histograms for each event.
  void UpdateTransientPlots(TDataContainer &dataContainer);

  // Get the histograms
  std::vector<THistogramArrayBase *> GetHistograms()
  {
    return fHistos;
  }

  inline void SetTHttpServer(THttpServer *s) { server = s; }

private:
  std::vector<THistogramArrayBase *> fHistos;
  TDetectorManager *detMan;
  TCanvasManager *canMan;
  TTreeManager *treeMan;
  THistManager *histMan;

  THttpServer *server;

  TH1 *h1;
  TTreeFormula *tf;
};

#endif
