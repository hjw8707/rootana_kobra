#ifndef TSSDAna_h
#define TSSDAna_h

#include <string>

#include "TKoBRAADCData.hxx"
#include "TClonesArray.h"

#include "TSSDData.hxx"
#include "TTreeManager.hxx"

class TSSDAna {
public:
    TSSDAna(const char* name, const char* _parfile);
    ~TSSDAna();

    void Clear();

    void SetParameters(const char* file);

    void SetData(std::vector<KoBRAADCMeasurement>& _data);
    void Analysis();
    TSSDData* Processing(int ch, uint32_t adc);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TSSDData* GetData(int i) { return static_cast<TSSDData*>((*outdata)[i]); }

    void SetTree();

private:
    static const int n = 16;

    std::string name;
    std::vector<uint32_t> data[n]; // data for each strip
    
    TClonesArray* outdata;; // processed data
    
    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers
    uint32_t chs[n];
    //        
    // parameters: [0,1] = [left, right]
    float offset[n];
    float factor[n];
    float ped[n];
    float adc_cut[2];
    ///////////////////////////////////////////////////////

};

#endif