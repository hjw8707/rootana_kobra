#ifndef TKoBRADIGHistograms_h
#define TKoBRADIGHistograms_h

#include <string>
#include "THistogramArrayBase.h"

/// Class for making histograms of KoBRADIG DIG data.
class TKoBRADIGHistograms : public THistogramArrayBase {
 public:
  TKoBRADIGHistograms(const char* _bankname);
  virtual ~TKoBRADIGHistograms(){};
  
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


