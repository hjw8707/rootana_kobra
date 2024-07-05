#ifndef TDetectorAna_h
#define TDetectorAna_h

#include "TGenericData.hxx"
#include "TClonesArray.h"

#include <vector>

class TDetectorAna
{
public:
    TDetectorAna() {}
    ~TDetectorAna() {}

    virtual void Clear() = 0;
    virtual void SetData(TGenericData *) = 0;
    virtual void SetParameters(const char *) = 0;

    virtual TClonesArray *GetDataArray() = 0;
    //inline const T *GetData(int i) = 0;
    virtual void PrintParameters() = 0;
    virtual void Analysis() = 0;
    virtual void SetTree() = 0;
};

#endif