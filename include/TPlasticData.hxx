#ifndef TPlasticData_h
#define TPlasticData_h

#include "TMath.h"
#include "TObject.h"

class TPlasticData : public TObject {
   public:
    TPlasticData() : tl(0), tr(0), tsum(0), tdiff(0), al(0), ar(0) {}
    TPlasticData(uint32_t _tl, uint32_t _tr, float _tsum, float _tdiff, float _al = 0, float _ar = 0)
        : tl(_tl), tr(_tr), tsum(_tsum), tdiff(_tdiff), al(_al), ar(_ar) {}
    virtual ~TPlasticData() {}

    inline float GetTAve() const { return tsum / 2.; }
    inline float GetAAveA() const { return (al + ar) / 2.; }
    inline float GetAAveG() const { return TMath::Sqrt(al * ar); }
    void Print();

    uint32_t tl, tr;
    float al, ar;
    float tsum, tdiff;

    ClassDef(TPlasticData, 1)
};

#endif