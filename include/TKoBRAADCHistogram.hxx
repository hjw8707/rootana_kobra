#ifndef TKoBRAADCHistograms_h
#define TKoBRAADCHistograms_h

#include <string>
#include "THistogramArrayBase.h"

/// Class for making histograms of KoBRAADC ADC data.
class TKoBRAADCHistograms : public THistogramArrayBase {
 public:
  TKoBRAADCHistograms(const char* _bankname);
  virtual ~TKoBRAADCHistograms(){};
  
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


