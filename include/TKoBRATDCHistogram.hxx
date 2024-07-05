#ifndef TKoBRATDCHistograms_h
#define TKoBRATDCHistograms_h

#include <string>
#include "THistogramArrayBase.h"

/// Class for making histograms of KoBRATDC TDC data.
class TKoBRATDCHistograms : public THistogramArrayBase {
 public:
  TKoBRATDCHistograms(const char* _bankname);
  virtual ~TKoBRATDCHistograms(){};
  
  /// Update the histograms for this canvas.
  void UpdateHistograms(TDataContainer& dataContainer);

  /// Take actions at begin run
  void BeginRun(int transition,int run,int time);

  /// Take actions at end run  
  void EndRun(int transition,int run,int time);

private:

  void CreateHistograms();
  std::string bankname;
    
};

#endif


