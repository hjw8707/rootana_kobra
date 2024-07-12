#ifndef THEADER_H
#define THEADER_H

#include "TObject.h"

#include <ctime>

class THeader: public TObject {

public:
  THeader(): fRunNum(-1), fStartTimeStamp(-1), fStopTimeStamp(-1) {}
  virtual ~THeader(){}

  void SetRunNum(Int_t r) { fRunNum = r; }
  Int_t GetRunNum() { return fRunNum; }

  void SetStartTimeStamp(Int_t t) { fStartTimeStamp = t; }
  Int_t GetStartTimeStamp() { return fStartTimeStamp; }

  void SetStopTimeStamp(Int_t t) { fStopTimeStamp = t; }
  Int_t GetStopTimeStamp() { return fStopTimeStamp; }

  Int_t GetElaspedTime() { return difftime(time_t(fStopTimeStamp), time_t(fStartTimeStamp)); }
  
private:
  Int_t fRunNum;

  Int_t fStartTimeStamp;
  Int_t fStopTimeStamp;

  ClassDef(THeader, 1);
};

#endif
