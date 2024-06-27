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

#include "TDetectorManager.hxx"

#include "TDetectorAna.hxx"
#include "TPPACAna.hxx"
#include "TLPPACAna.hxx"
#include "TPlasticAna.hxx"
#include "TDPlasticAna.hxx"
#include "TSSDAna.hxx"

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

private:
  std::vector<THistogramArrayBase *> fHistos;
  TDetectorManager *detMan;

  TLPPACAna *ppac_f1u, *ppac_f1d;
  TPPACAna *ppac_f2u, *ppac_f2d, *ppac_f3u, *ppac_f3d, *ppac_sc;
  TPlasticAna *pla_f2, *pla_f3;
  TPlasticAna *ppt_f1, *ppt_f2u, *ppt_f2d, *ppt_f3u, *ppt_f3d;
  TSSDAna *ssd_f2, *ssd_f3;
  TDPlasticAna *dpla_f2, *dpla_f3;
};

#endif
