#ifndef TLPPACAna_h
#define TLPPACAna_h

#include <string>

#include "TClonesArray.h"
#include "TDetectorAna.hxx"
#include "TKoBRATDCData.hxx"
#include "TPPACData.hxx"
#include "TTreeManager.hxx"

class TLPPACAna : public TDetectorAna {
   public:
    TLPPACAna(const char *name, const char *_parfile);
    ~TLPPACAna();

    void Clear();

    void SetParameters(const char *file);

    // void SetData(std::vector<KoBRATDCMeasurement> &_data);
    void SetData(TGenericData *data);
    void Analysis();
    TPPACData *Processing(uint32_t tx1, uint32_t tx2, uint32_t ty1, uint32_t ty2, uint32_t ta, int lcr);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TPPACData *GetData(int i) { return static_cast<TPPACData *>((*outdata)[i]); }
    inline TClonesArray *GetDataArray() { return outdata; }

    void SetTree();

   private:
    static const int n = 9;

    std::string name;
    std::vector<uint32_t> data[n];  // data for each XUP, XDN, YUP, YDN, Anode channels

    TClonesArray *outdata;  // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers: [0,1,2,3] = [x_up, x_dn, y_up, y_dn, anode]
    std::vector<uint32_t> chs;
    //
    // parameters: [0,1,2,3] = [xl, xc, xr, y]
    std::vector<float> offset;
    std::vector<float> factor;

    float txsum_offset[3];  // [left, center, right]
    float tdc_cut[2];
    float tsum_cut[2][2];  // [0] x, [1] y
    ///////////////////////////////////////////////////////
};

#endif