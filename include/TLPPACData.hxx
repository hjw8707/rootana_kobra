#ifndef TLPPACData_h
#define TLPPACData_h

#include "TObject.h"

class TLPPACData: public TObject {
    public:
    TLPPACData():
            txl1(0), txl2(0), txc1(0), txc2(0), txr1(0), txr2(0), ty1(0), ty2(0), ta(0), 
            x(0), y(0) 
            {}    
    TLPPACData(uint32_t _txl1, uint32_t _txl2, uint32_t _txc1, uint32_t _txc2, 
    uint32_t _txr1, uint32_t _txr2, uint32_t _ty1, uint32_t _ty2,
    uint32_t _ta, float _x, float _y):
            txl1(_txl1), txl2(_txl2), txc1(_txc1), txc2(_txc2), txr1(_txr1), txr2(_txr2), 
            ty1(_ty1), ty2(_ty2), ta(_ta), 
            x(_x), y(_y)
            {}
    virtual ~TLPPACData() {}

   uint32_t GetTXLSum(); 
   uint32_t GetTXCSum(); 
   uint32_t GetTXRSum(); 
   uint32_t GetTYSum();

    void Print();

    uint32_t txl1, txl2, txc1, txc2, txr1, txr2, ty1, ty2, ta;
    float x, y;

    ClassDef(TLPPACData,1)
};

#endif