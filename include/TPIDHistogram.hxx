#ifndef TPIDHistograms_h
#define TPIDHistograms_h

#include <string>
#include "THistogramArrayBase.h"
#include "TClonesArray.h"

/// Class for making histograms of PID TDC data.
class TPIDHistograms : public THistogramArrayBase {
 public:
  TPIDHistograms(const char* _name, TClonesArray* _f2pladata, TClonesArray* _f3pladata, TClonesArray* _f2ssddata, TClonesArray* _f3ssddata);
  virtual ~TPIDHistograms(){};
  
  /// Update the histograms for this canvas.
  void UpdateHistograms(TDataContainer& dataContainer);

  /// Take actions at begin run
  void BeginRun(int transition,int run,int time);

  /// Take actions at end run  
  void EndRun(int transition,int run,int time);

private:

  void CreateHistograms();
  std::string name;
  TClonesArray *f2pladata, *f3pladata, *f2ssddata, *f3ssddata;
};

#endif


