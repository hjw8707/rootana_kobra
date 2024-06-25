#ifndef TPlasticAna_h
#define TPlasticAna_h

#include <string>

#include "TKoBRATDCData.hxx"
#include "TClonesArray.h"

#include "TPlasticData.hxx"

class TPlasticAna {
public:
    TPlasticAna(const char* name, const char* _parfile);
    ~TPlasticAna();

    void Clear();

    void SetParameters(const char* file);

    void SetData(std::vector<KoBRATDCMeasurement>& _data);
    void Analysis();
    TPlasticData* Processing(uint32_t tl, uint32_t tr);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TPlasticData* GetData(int i) { return static_cast<TPlasticData*>((*outdata)[i]); }

private:
    static const int n = 2;

    std::string name;
    std::vector<uint32_t> data[n]; // data for each XUP, XDN, YUP, YDN, Anode channels
    
    TClonesArray* outdata;; // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers: [0,1] = [left, right]
    uint32_t chs[n];
    //        
    // parameters: [0,1] = [left, right]
    float offset[2];
    float factor[2];
    float tdc_cut[2];
    ///////////////////////////////////////////////////////

};

#endif