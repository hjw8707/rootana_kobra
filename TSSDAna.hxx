#ifndef TSSDAna_h
#define TSSDAna_h

#include <string>

#include "TKoBRAADCData.hxx"
#include "TClonesArray.h"

#include "TDetectorAna.hxx"
#include "TSSDData.hxx"
#include "TTreeManager.hxx"

class TSSDAna : public TDetectorAna
{
public:
    TSSDAna(const char *_name, const char *_parfile, int _n);
    ~TSSDAna();

    void Clear();

    void SetParameters(const char *file);

    //void SetData(std::vector<KoBRAADCMeasurement> &_data);
    void SetData(TGenericData* data);
    void Analysis();
    TSSDData *Processing(int ch, uint32_t adc);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TSSDData *GetData(int i) { return static_cast<TSSDData *>((*outdata)[i]); }

    void SetTree();

private:
    int n; // number of channels

    std::string name;
    std::vector<std::vector<uint32_t>> data; // data for each strip

    TClonesArray *outdata;
    ; // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers
    std::vector<uint32_t> chs;
    //
    // parameters: [0,1] = [left, right]
    std::vector<float> offset;
    std::vector<float> factor;
    std::vector<float> ped;
    float adc_cut[2];
    ///////////////////////////////////////////////////////
};

#endif