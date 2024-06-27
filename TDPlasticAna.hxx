#ifndef TDPlasticAna_h
#define TDPlasticAna_h

#include <string>
#include <utility>

#include "TKoBRADIGData.hxx"
#include "TClonesArray.h"

#include "TDetectorAna.hxx"
#include "TPlasticData.hxx"
#include "TTreeManager.hxx"

class TDPlasticAna : public TDetectorAna
{
public:
    TDPlasticAna(const char *name, const char *_parfile, int _n = 2);
    ~TDPlasticAna();

    void Clear();

    void SetParameters(const char *file);

    //void SetData(std::vector<KoBRADIGMeasurement> &_data);
    void SetData(TGenericData* data);
    void Analysis();
    TPlasticData *Processing(uint32_t tl, uint32_t tr, uint32_t al, uint32_t ar);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TPlasticData *GetData(int i) { return static_cast<TPlasticData *>((*outdata)[i]); }

    void SetTree();

private:
    int n;

    std::string name;
    std::vector<std::pair<uint32_t, uint32_t>> data[2]; // data for each Left, Right

    TClonesArray *outdata; // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers: [0,1] = [left, right]
    std::vector<uint32_t> chs;
    //
    // parameters: [0,1] = [left, right]
    std::vector<float> t_offset;
    std::vector<float> t_factor;
    std::vector<float> a_offset;
    std::vector<float> a_factor;
    float tdc_cut[2];
    float adc_cut[2];
    ///////////////////////////////////////////////////////
};

#endif