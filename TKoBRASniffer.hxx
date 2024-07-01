#ifndef TKoBRASniffer_h
#define TKoBRASniffer_h

#include "TRootSniffer.h"
#include <iostream>

class TKoBRASniffer : public TRootSniffer
{
public:
    TKoBRASniffer();
    ~TKoBRASniffer();

    Bool_t HistClear();
    Bool_t HistReload();

    ClassDef(TKoBRASniffer, 1);
};

#endif