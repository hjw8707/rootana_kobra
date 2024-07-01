#ifndef TSSDHistograms_h
#define TSSDHistograms_h

#include <string>
#include "THistogramArrayBase.h"
#include "TClonesArray.h"

#include "TSSDAna.hxx"

/// Class for making histograms of SSD TDC data.
class TSSDHistograms : public THistogramArrayBase {
 public:
  TSSDHistograms(const char* _name, TClonesArray* _data);
  virtual ~TSSDHistograms(){};
  
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


