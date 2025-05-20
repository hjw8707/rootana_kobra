#ifndef TScalerData_h
#define TScalerData_h

#include <iostream>

#include "TObject.h"

class TScalerData : public TObject {
   public:
    TScalerData() : ts(0) {}
    TScalerData(uint64_t _ts, uint32_t _ppac, uint32_t _f2pl, uint32_t _f3pl, uint32_t _hwtriga, uint32_t _hwtrigb,
                uint32_t _refp, uint32_t _reft)
        : ts(_ts),
          ppac(_ppac),
          f2pl(_f2pl),
          f3pl(_f3pl),
          hwtriga(_hwtriga),
          hwtrigb(_hwtrigb),
          refp(_refp),
          reft(_reft) {}
    virtual ~TScalerData() {}

    void Print();
    void PrintSingleLine(std::ostream &os = std::cout);

    uint64_t ts;
    uint32_t ppac, f2pl, f3pl, hwtriga, hwtrigb, refp, reft;

    ClassDef(TScalerData, 1)
};

#endif
