#ifndef TPlasticAna_h
#define TPlasticAna_h

#include <string>

#include "TClonesArray.h"
#include "TDetectorAna.hxx"
#include "TKoBRATDCData.hxx"
#include "TPlasticData.hxx"
#include "TTreeManager.hxx"

class TPlasticAna : public TDetectorAna {
   public:
    TPlasticAna(const char *name, const char *_parfile, int _n = 2);
    ~TPlasticAna();

    void Clear();

    void SetParameters(const char *file);

    // void SetData(std::vector<KoBRATDCMeasurement> &_data);
    void SetData(TGenericData *data);
    void Analysis();
    TPlasticData *Processing(uint32_t tl, uint32_t tr);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TPlasticData *GetData(int i) { return static_cast<TPlasticData *>((*outdata)[i]); }
    inline TClonesArray *GetDataArray() { return outdata; }

    void SetTree();

   private:
    int n;

    std::string name;
    std::vector<uint32_t> data[2];  // data for each XUP, XDN, YUP, YDN, Anode channels

    TClonesArray *outdata;  // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers: [0,1] = [left, right]
    std::vector<uint32_t> chs;
    //
    // parameters: [0,1] = [left, right]
    std::vector<float> offset;
    std::vector<float> factor;
    float tdc_cut[2];
    float tdiff_cut[2];
    ///////////////////////////////////////////////////////
};

#endif