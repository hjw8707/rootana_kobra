#ifndef TTrackHistograms_h
#define TTrackHistograms_h

#include <string>
#include "THistogramArrayBase.h"
#include "TClonesArray.h"

/// Class for making histograms of Track TDC data.
class TTrackHistograms : public THistogramArrayBase {
 public:
  TTrackHistograms(const char* _name, TClonesArray* _ppacup, TClonesArray* _ppacdn, float _dist);
  virtual ~TTrackHistograms(){};
  
  /// Update the histograms for this canvas.
  void UpdateHistograms(TDataContainer& dataContainer);

  /// Take actions at begin run
  void BeginRun(int transition,int run,int time);

  /// Take actions at end run  
  void EndRun(int transition,int run,int time);

private:

  void CreateHistograms();
  std::string name;
  TClonesArray *ppacup, *ppacdn;
  float dist;
};

#endif


