#ifndef TAnaManager_h
#define TAnaManager_h

// Use this list here to decide which type of equipment to use.

// #define USE_V792
// #define USE_V1190
// #define USE_L2249
// #define USE_AGILENT
// #define USE_V1720
// #define USE_V1720_CORRELATIONS
// #define USE_V1730DPP
// #define USE_V1730RAW
// #define USE_DT724
// #define USE_TRB3
// #define USE_CAMACADC

#include "TTree.h"

#include "TKoBRADIGData.hxx"
#include "TKoBRAADCData.hxx"
#include "TKoBRATDCData.hxx"
#include "TKoBRATDCHistogram.h"
#include "TKoBRAADCHistogram.h"
#include "TKoBRADIGHistogram.h"
#include "TPPACHistogram.h"
#include "TPlasticHistogram.h"
#include "TSSDHistogram.h"
#include "TPIDHistogram.h"
#include "TTrackHistogram.h"

#include "TDetectorManager.hxx"
#include "TCanvasManager.hxx"
#include "THistManager.hxx"
#include "TTreeManager.hxx"

#include "THttpServer.h"

/// This is an example of how to organize a set of different histograms
/// so that we can access the same information in a display or a batch
/// analyzer.
/// Change the set of ifdef's above to define which equipment to use.
///
/// We simplify a lot of code by mostly using an array of THistogramArrayBase classes
/// for storing different histograms.
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
