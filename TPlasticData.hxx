#ifndef TPlasticData_h
#define TPlasticData_h

#include "TObject.h"

class TPlasticData: public TObject {
    public:
    TPlasticData():
            tl(0), tr(0), tsum(0), tdiff(0)
            {}    
    TPlasticData(uint32_t _tl, uint32_t _tr, float _tsum, float _tdiff):
            tl(_tl), tr(_tr), tsum(_tsum), tdiff(_tdiff)
            {}
    virtual ~TPlasticData() {}

    void Print();

    uint32_t tl, tr;
    float tsum, tdiff;

    ClassDef(TPlasticData,1)
};

#endif