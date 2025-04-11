#ifndef KOBRA_HXX
#define KOBRA_HXX

#include <iostream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TClonesArray.h"
#include "TCutG.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TObject.h"
#include "TPad.h"

class THeader;

class KOBRA : public TObject {
   public:
    enum class Expt { Comm, Phys };

   public:
    KOBRA(Expt expt);
    KOBRA(Expt expt, const char *file);
    KOBRA(Expt expt, std::string file);
    KOBRA(Expt expt, std::vector<std::string> files);
    KOBRA(Expt expt, int run);
    KOBRA(Expt expt, std::vector<int> runs);
    KOBRA(Expt expt, int runb, int rune);

    virtual ~KOBRA();

    void Initilize();

    Bool_t LoadTree(const char *file);
    Bool_t LoadTree(std::string file);
    Bool_t LoadTree(std::vector<std::string> files);

    Bool_t LoadTree(Expt expt, int run);
    Bool_t LoadTree(Expt expt, std::vector<int> runs);
    Bool_t LoadTree(Expt expt, int runb, int rune);

    inline TChain *GetTree() { return tree; }

    void RunSetting(int run);

    Bool_t LoadDB(const char *file);

    void SetAlias();

    inline void SetBrho(Double_t br) {
        centBrho = br;
        SetAlias();
    }
    inline Double_t GetBrho() { return centBrho; }

    inline void SetTOFOffset(Double_t off) {
        tofOff = off;
        SetAlias();
    }
    inline Double_t GetTOFOffset() { return tofOff; }

    inline void SetUseF1(Bool_t us = true) {
        useF1 = us;
        SetAlias();
    }
    inline Bool_t GetUseF1() { return useF1; }

    inline void SetUseF2orF3(Bool_t us) {
        useF2orF3 = us;
        SetAlias();
    }
    inline Bool_t GetUseF2orF3() { return useF2orF3; }

    inline void SetUseF2() { SetUseF2orF3(true); }
    inline void SetUseF3() { SetUseF2orF3(false); }

    inline void SetUseSSDorPla(Bool_t us) {
        useSSDorPla = us;
        SetAlias();
    }
    inline Bool_t GetUseSSDorPla() { return useSSDorPla; }

    inline void SetUseSSD() {
        SetUseSSDorPla(true);
        AddCuts("./cut");
    }
    inline void SetUsePla() {
        SetUseSSDorPla(false);
        AddCuts("./cut");
        // AddCuts("./cut_pla");
    }
    // inline void SetUsePla() { SetUseSSDorPla(false); }

    inline void SetZ(Double_t a, Double_t b) { tree->SetAlias("Z", Form("%f*z+%f", a, b)); }
    void SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2);

    inline void SetAoQ(Double_t a, Double_t b) { tree->SetAlias("AoQ", Form("%f*aoq+%f", a, b)); }
    void SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2);

    inline void SetGCut(const char *_c) { gcut = _c; }
    inline const char *GetGCut() { return gcut.c_str(); }

    void DrawPID0(const char *cut = NULL);
    void DrawPID(Int_t show = 0, const char *cut = NULL);   // show = 0: nothing, 1: count, 2: pps
    void DrawPIDC(Int_t show = 0, const char *cut = NULL);  // show = 0: nothing, 1: count, 2: pps

    std::vector<TH1 *> DrawXDist(const char *cut = NULL, bool flagDraw = true);
    TH1 *DrawMomDist(const char *cut = NULL, Bool_t flagRate = false, Bool_t flagDraw = true, Double_t binSize = 0.5,
                     Bool_t flagOff = false);
    TGraphErrors *GetMomDistGraph(Double_t center = 0, const char *cut = NULL, Double_t binSize = 0.5,
                                  Double_t leftLimit = -4, Double_t right = 4, Bool_t flagOff = false,
                                  Double_t scale = 1, Bool_t flagUsef1 = true);
    std::vector<TGraphErrors *> GetMomDistGraphs(Double_t center, Double_t binSize, Double_t leftLimit,
                                                 Double_t rightLimit, Bool_t flagOff, Int_t bias = 0);
    std::vector<TGraphErrors *> GetMomDistGraphsNe(std::vector<std::string> isos, Double_t center, Double_t binSize,
                                                   Double_t leftLimit, Double_t rightLimit, Bool_t flagOff,
                                                   Int_t bias = 0, Bool_t flagUseF1 = true);

    void DrawPPACEff(const char *cut = NULL);
    void PrintSetting(std::ostream &out = std::cout);

    void CountIsotopes();
    void RateIsotopes();

    void AddCut(const char *filename);
    void AddCuts(const char *path);

    void DrawCut(const char *cut, bool flagName = false);
    void DrawAllCuts(bool flagName = false);

    void PrintCutCoord(TCutG *cut, Double_t xoff = 0, Double_t yoff = 0);

    Double_t GetElapsedTime();

    Long64_t GetEntries(const char *selection);

    TGraph *PPACRate();
    TGraph *TriggerRate();

    Double_t GetPPACEff(Int_t id, const char *cut = NULL, bool flagTable = false, std::string iso = "", int bias = 0);

    Double_t GetLiveTime();

    void ApplyOffsetToCut(Double_t xoff, Double_t yoff = 0, Double_t xscale = -1, Double_t yscale = -1);
    void SetCutStyle(Int_t color = 1, Int_t width = 1, Int_t style = 2);

    TCutG *Make2DCut(const char *name, const char *title);

    inline THeader *GetHeader(int i) { return headers[i]; }

    void AnaAtOnce(const char *name, std::vector<std::string> iso);
    void AnaAtOnceMom(const char *name, Double_t delta, std::vector<std::string> iso = {});

    // ... existing code ...
    void FitPIDLocus(const char *name, int nbinsX = 1000, int nbinsY = 1000);
    void Fit2DGaussian(TH2 *hist);

    ////////////////////////////////////////////////////////
    // SSD calibration
    void F3SSDPedestal(const char *cut, int low = 0, int high = 200);
    void F3SSDPeak(const char *cut, int low = 200, int high = 2000);
    ////////////////////////////////////////////////////////

    TChain *tree;
    std::map<Int_t, Double_t> brhoMap;
    std::map<Int_t, Double_t> f1slitMap;

    //////////////////////////////////////////////////////////////////////////////
    // run group for each isotope (defined at kobra.cxx)
    static std::vector<int> o18, o19, o20, o21, o22, ne24, ne25, ne26;
    static std::vector<std::string> o18_iso, o19_iso, o20_iso, o21_iso, o22_iso, ne24_iso, ne25_iso, ne26_iso;
    static std::vector<int> mom_14, mom_12, mom_10, mom_08, mom_07, mom_06, mom_05, mom_04;
    static std::vector<int> mom00, mom04, mom07;
    /////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // run group for each isotope (defined at kobra.cxx)
    static std::vector<int> mon_10, monn_08, mon_08, mon_07, mon_06, mon_05, mon_04, monn_04, mon_03, mon_02, mon_01,
        mon00, mon01, mon02, mon04, mon06, mon10;
    //////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    // for mom dist. analysis
    static void MomDistAnalysis();
    static void NeMomDistAnalysis(bool flagFast = true);
    static void CrossSectionAnalysis();
    ////////////////////////////////////////////////////////

    static void PPACEffCurve();

    std::map<std::string, TCutG *> cutgs;
    std::map<std::string, std::map<Int_t, Double_t>> effF1UPPACX;

   private:
    Expt expt;
    std::vector<Int_t> runNs;
    std::vector<THeader *> headers;

    Double_t centBrho;
    Double_t tofOff;

    Bool_t useF1;
    Bool_t useF2orF3;    // true = F2, false = F3
    Bool_t useSSDorPla;  // true = SSD, false = Pla

    std::string gcut;

    std::vector<std::string> split(const std::string &s, char delimiter, bool flagEmpty = true);

    TClonesArray *ca;

    ClassDef(KOBRA, 1)
};

#endif
