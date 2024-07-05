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

    Bool_t HistFit(const char* name);

    void SetRunNumber(int run);
    void SetRunStatus(bool running);

    ClassDef(TKoBRASniffer, 1);

private:
    TObjString *runnum;
    TObjString *runstat;
};

#endif