#ifndef TPlasticHistograms_h
#define TPlasticHistograms_h

#include <string>
#include "THistogramArrayBase.h"
#include "TClonesArray.h"

#include "TPlasticAna.hxx"

/// Class for making histograms of Plastic TDC data.
class TPlasticHistograms : public THistogramArrayBase {
 public:
  TPlasticHistograms(const char* _name, TClonesArray* _data);
  virtual ~TPlasticHistograms(){};
  
  /// Update the histograms for this canvas.
  void UpdateHistograms(TDataContainer& dataContainer);

  /// Take actions at begin run
  void BeginRun(int transition,int run,int time);

  /// Take actions at end run  
  void EndRun(int transition,int run,int time);

private:

  void CreateHistograms();
  std::string name;
  TClonesArray* data;
};

#endif


