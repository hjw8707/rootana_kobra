#ifndef CURRY_HXX
#define CURRY_HXX

#include "TObject.h"
#include "TChain.h"
#include "TCutG.h"
#include "TClonesArray.h"
#include "TGraph.h"
#include "TPad.h"

#include <vector>
#include <iostream>
#include <string>

#include "TPad.h"

class CURRY : public TObject
{
public:
  CURRY(){};
  virtual ~CURRY(){};

  void GetEvent();
  Bool_t Run(TPad *pad, Double_t *x, Double_t *y, Bool_t *end = NULL);
  Bool_t Run(Double_t *x, Double_t *y, Bool_t *end = NULL);
  
  static CURRY* GetInstance();
  static CURRY* curry;

private:
  Double_t fX, fY;
  Bool_t fEnd;
  
  ClassDef(CURRY, 1)
};

#endif
