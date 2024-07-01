#ifndef TPPACHistograms_h
#define TPPACHistograms_h

#include <string>
#include "THistogramArrayBase.h"
#include "TClonesArray.h"

#include "TPPACAna.hxx"

/// Class for making histograms of PPAC TDC data.
class TPPACHistograms : public THistogramArrayBase {
 public:
  TPPACHistograms(const char* _name, TClonesArray* _data);
  virtual ~TPPACHistograms(){};
  
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


