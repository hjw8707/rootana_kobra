#ifndef TPPACData_h
#define TPPACData_h

#include "TObject.h"

class TPPACData: public TObject {
    public:
    TPPACData():
            tx1(0), tx2(0), ty1(0), ty2(0), ta(0), 
            x(0), y(0) 
            {}    
    TPPACData(uint32_t _tx1, uint32_t _tx2, uint32_t _ty1, uint32_t _ty2,
    uint32_t _ta, float _x, float _y):
            tx1(_tx1), tx2(_tx2), ty1(_ty1), ty2(_ty2), ta(_ta), 
            x(_x), y(_y)
            {}
    virtual ~TPPACData() {}

   uint32_t GetTXSum(); 
   uint32_t GetTYSum();

    void Print();

    uint32_t tx1, tx2, ty1, ty2, ta;
    float x, y;

    ClassDef(TPPACData,1)
};

#endif