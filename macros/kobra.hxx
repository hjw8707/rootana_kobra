#ifndef KOBRA_HXX
#define KOBRA_HXX

#include "TObject.h"
#include "TChain.h"

#include <vector>
#include <string>

class KOBRA : public TObject
{

public:
    KOBRA();
    KOBRA(const char *file);
    KOBRA(std::string file);
    KOBRA(std::vector<std::string> files);
    KOBRA(int run);
    KOBRA(std::vector<int> runs);
    KOBRA(int runb, int rune);

    virtual ~KOBRA();

    void Initilize();

    Bool_t LoadTree(const char *file);
    Bool_t LoadTree(std::string file);
    Bool_t LoadTree(std::vector<std::string> files);

    Bool_t LoadTree(int run);
    Bool_t LoadTree(std::vector<int> runs);
    Bool_t LoadTree(int runb, int rune);

    inline TChain *GetTree() { return tree; }

    void RunSetting(int run);

    Bool_t LoadDB(const char *file);

    void SetAlias();

    inline void SetBrho(Double_t br)
    {
        centBrho = br;
        SetAlias();
    }
    inline Double_t GetBrho() { return centBrho; }

    inline void SetTOFOffset(Double_t off)
    {
        tofOff = off;
        SetAlias();
    }
    inline Double_t GetTOFOffset() { return tofOff; }

    inline void SetUseF1(Bool_t us)
    {
        useF1 = us;
        SetAlias();
    }
    inline Bool_t GetUseF1() { return useF1; }

    inline void SetUseF2orF3(Bool_t us)
    {
        useF2orF3 = us;
        SetAlias();
    }
    inline Bool_t GetUseF2orF3() { return useF2orF3; }

    inline void SetUseF2() { SetUseF2orF3(true); }
    inline void SetUseF3() { SetUseF2orF3(false); }

    inline void SetZ(Double_t a, Double_t b) { tree->SetAlias("Z", Form("%f*z+%f",a,b));}
    void SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2);

    inline void SetAoQ(Double_t a, Double_t b) { tree->SetAlias("AoQ", Form("%f*aoq+%f",a,b));}
    void SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2);

    void DrawPID0(const char *cut = NULL);
    void DrawPID(const char *cut = NULL);
    void DrawPIDC(const char *cut = NULL);

    TChain *tree;
    std::map<Int_t, Double_t> brhoMap;
private:
    Int_t runN;

    Double_t centBrho;
    Double_t tofOff;

    Bool_t useF1;
    Bool_t useF2orF3; // true = F2, false = F3

    std::string gcut;

    std::vector<std::string> split(const std::string &s, char delimiter);
    ClassDef(KOBRA, 1)
};

#endif