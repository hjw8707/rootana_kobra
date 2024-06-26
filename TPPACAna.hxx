#ifndef TPPACAna_h
#define TPPACAna_h

#include <string>

#include "TKoBRATDCData.hxx"
#include "TClonesArray.h"

#include "TPPACData.hxx"

#include "TTreeManager.hxx"

class TPPACAna {
public:
    TPPACAna(const char* name, const char* _parfile);
    ~TPPACAna();

    void Clear();

    void SetParameters(const char* file);

    void SetData(std::vector<KoBRATDCMeasurement>& _data);
    void Analysis();
    TPPACData* Processing(uint32_t tx1, uint32_t tx2, 
                            uint32_t ty1, uint32_t ty2,
                            uint32_t ta);

    void PrintParameters();
    void PrintData();

    void PrintOutdata();

    inline const TPPACData* GetData(int i) { return static_cast<TPPACData*>((*outdata)[i]); }

    void SetTree();

private:
    static const int n = 5;

    std::string name;
    std::vector<uint32_t> data[n]; // data for each XUP, XDN, YUP, YDN, Anode channels
    
    TClonesArray* outdata;; // processed data

    bool flagSet;
    bool flagData;
    ///////////////////////////////////////////////////////
    // channel numbers: [0,1,2,3] = [x_up, x_dn, y_up, y_dn, anode]
    uint32_t chs[n];
    //        
    // parameters: [0,1] = [x, y]
    float offset[2];
    float factor[2];
    float tdc_cut[2];
    ///////////////////////////////////////////////////////

};

#endif