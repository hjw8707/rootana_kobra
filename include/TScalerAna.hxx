#ifndef TScalerAna_h
#define TScalerAna_h

#include <string>

#include "TKoBRASCAData.hxx"
#include "TClonesArray.h"

#include "TDetectorAna.hxx"
#include "TScalerData.hxx"

#include "TTreeManager.hxx"

class TScalerAna: public TDetectorAna {
public:
  TScalerAna(const char* name, const char* _parfile);
  ~TScalerAna();

  void Clear();

  void SetParameters(const char* file);

  void SetData(TGenericData* data);        
  void Analysis();
  TScalerData* Processing(uint64_t _ts, uint32_t _ppac, uint32_t _f2pl, uint32_t _f3pl,
			  uint32_t _hwtriga, uint32_t _hwtrigb, uint32_t _refp, uint32_t _reft);

  void PrintParameters();
  void PrintData();

  void PrintOutdata();

  inline const int GetNData() { return outdata->GetEntriesFast(); }
  inline TScalerData* GetData(int i) { return static_cast<TScalerData*>((*outdata)[i]); }
  inline TClonesArray* GetDataArray() { return outdata; }

  void SetTree();

private:
  std::string name;
  std::vector<TKoBRASCAData*> data; // data for each XUP, XDN, YUP, YDN, Anode channels
    
  TClonesArray* outdata;; // processed data

  bool flagSet;
  bool flagData;

};

#endif
