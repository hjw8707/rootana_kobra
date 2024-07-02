#ifndef TKoBRASniffer_h
#define TKoBRASniffer_h

#include "TRootSniffer.h"
#include <iostream>

#include "TObjString.h"
class TKoBRASniffer : public TRootSniffer
{
public:
    TKoBRASniffer();
    ~TKoBRASniffer();

    Bool_t HistClear();
    Bool_t HistReload();

    void SetRunNumber(int run);

    ClassDef(TKoBRASniffer, 1);

private:
    TObjString *runnum;
};

#endif