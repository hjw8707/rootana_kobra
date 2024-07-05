#ifndef TSSDData_h
#define TSSDData_h

#include "TObject.h"

class TSSDData : public TObject
{
public:
        TSSDData() : ch(0), adc(0), acal(0)
        {
        }
        TSSDData(int _ch, uint32_t _adc, float _acal) : ch(_ch), adc(_adc), acal(_acal)
        {
        }
        virtual ~TSSDData() {}

        void Print();

        int ch;
        uint32_t adc;
        float acal;

        ClassDef(TSSDData, 1)
};

#endif