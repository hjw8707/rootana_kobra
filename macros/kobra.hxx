#ifndef KOBRA_HXX
#define KOBRA_HXX

#include <iostream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TClonesArray.h"
#include "TCut.h"
#include "TCutG.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH2.h"
#include "TMultiGraph.h"
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
  KOBRA(Expt expt, std::vector<int> runs, float brho);
  KOBRA(Expt expt, int runb, int rune);

  // constructor for mdis runs
  KOBRA(Expt expt, std::map<int, std::vector<std::vector<int>>> runmap);

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

  std::vector<Int_t> GetRunNs() { return runNs; }
  std::vector<std::string> GetCuts();

  void SetAlias();

  void SetStyleAhn();

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
    // AddCuts("./cut");
    AddCuts("./cut_pla");
  }
  // inline void SetUsePla() { SetUseSSDorPla(false); }

  inline void SetZ(Double_t a, Double_t b) { tree->SetAlias("Z", Form("%f*z+%f", a, b)); }
  void SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2);

  inline void SetAoQ(Double_t a, Double_t b) { tree->SetAlias("AoQ", Form("%f*aoq+%f", a, b)); }
  void SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2);

  inline void SetGCut(const char *_c) { gcut = _c; }
  inline const char *GetGCut() { return gcut.c_str(); }

  void SetPathLength(Double_t len = -1);
  inline Double_t GetPathLength() { return pathLength; }

  TH2 *DrawPID0(const char *cut = NULL);
  TH2 *DrawPID(Int_t show = 0, const char *cut = NULL);   // show = 0: nothing, 1: count, 2: pps
  TH2 *DrawPIDC(Int_t show = 0, const char *cut = NULL);  // show = 0: nothing, 1: count, 2: pps

  std::vector<TH1 *> DrawXDist(const char *cut = NULL, bool flagDraw = true);
  TH1 *DrawMomDist(const char *cut = NULL, Bool_t flagRate = false, Bool_t flagDraw = true, Int_t nBin = 32,
                   Double_t lowLim = -160, Double_t highLim = 160);
  TGraphErrors *GetMomDistGraph(Double_t center = 0, const char *cut = NULL, Double_t momBinSize = 0.5,
                                Double_t momLowLim = -4, Double_t momUppLim = 4, Double_t scale = 1,
                                Bool_t flagUsef1 = true);
  std::map<std::string, TGraphErrors *> GetMomDistGraphs(std::vector<std::string> iso, Double_t center,
                                                         Double_t f1SlitLowLim, Double_t f1SlitUppLim, Int_t bias = 0,
                                                         Bool_t flagUseF1 = true);

  void DrawPPACTimings(bool flagLog = false, const char *cut = NULL, bool flagUseF1X = true);
  void DrawPPACMultiHits(const char *cut = NULL, bool flagUseF1X = true);
  void DrawPPACTSum(bool flagLog = false, const char *cut = NULL, bool noAnodeCut = false, bool flagFit = false,
                    bool flagUseF1X = true);
  void DrawPPACCorrelation(const char *ppac, const char *cut = NULL);

  void CheckF1PPAC(bool flagLog = false, bool flagUseF1X = true);

  void DrawPPACEff(const char *cut = NULL, std::ostream &out = std::cout);
  void PrintSetting(std::ostream &out = std::cout);

  void CountIsotopes(std::ostream &out = std::cout, bool flagTitle = true);
  void RateIsotopes(std::ostream &out = std::cout, bool flagTitle = true);
  void CountIsotopesRunByRun(std::ostream &out = std::cout, bool flagTitle = true, bool flagHeader = true);

  void AddCut(const char *filename);
  void AddCuts(const char *path);

  Long64_t Draw(const char *name, const char *cut = NULL, const char *option = "");

  void DrawCut(const char *cut, bool flagName = false);
  void DrawAllCuts(bool flagName = false);

  void PrintCutCoord(TCutG *cut, Double_t xoff = 0, Double_t yoff = 0);

  Double_t GetElapsedTime();

  Long64_t GetEntries(const char *selection);

  TMultiGraph *PPACRate(int gap = 10, int start = 0);
  TGraph *TriggerRate(int gap = 10);

  Double_t GetPPACEff(Int_t id, const char *cut = NULL, bool flagTable = false, std::string iso = "", int bias = 0);

  Double_t GetLiveTime();

  void ApplyOffsetToCut(Double_t xoff, Double_t yoff = 0, Double_t xscale = -1, Double_t yscale = -1);
  void SetCutStyle(Int_t color = 1, Int_t width = 1, Int_t style = 2);

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

  ////////////////////////////////////////////////////////
  // SSD, Pla check
  TH1 *F3PlaAccHis(const char *hname, const char *cut = NULL, bool flagDraw = true);
  TH1 *F3SSDAccHis(const char *hname, const char *cut = NULL, bool flagDraw = true);
  ////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////
  // Draw Length Distribution
  void DrawLength(const char *bname, const char *cut = NULL, bool flag01 = false, int nbins = 10);
  ////////////////////////////////////////////////////////

  void ExtractScalerInfo(bool flagPrint = false, const char *filename = nullptr);

  void DrawBrhoTimeInfo(TVirtualPad *pad, Double_t brho, Double_t time);

  TChain *tree;
  std::map<Int_t, Double_t> brhoMap;
  std::map<Int_t, Double_t> f1slitMap;

  //////////////////////////////////////////////////////////////////////////////
  // run group for each isotope (defined at kobra.cxx)
  static std::vector<int> o18, o19, o20, o21, o22, ne24, ne25, ne26;
  static std::vector<int> o_all, ne_all, cs_all;
  static std::vector<int> o22bl, o21bl, o20bl, totbl;
  static std::vector<std::string> o18_iso, o19_iso, o20_iso, o21_iso, o22_iso, ne24_iso, ne25_iso, ne26_iso;
  static std::vector<int> mom_14, mom_12, mom_10, mom_08, mom_07, mom_06, mom_05, mom_04;
  static std::vector<int> mom00, mom04, mom07;

  static std::map<std::string, std::vector<int>> csruns;
  static std::map<std::string, std::vector<std::string>> csiso;
  /////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // run group for each isotope (defined at kobra.cxx)
  static std::vector<int> mon_10, monn_08, mon_08, mon_07, mon_06, mon_05, mon_04, monn_04, mon_03, mon_02, mon_01,
      mon00, mon01, mon02, mon04, mon06, mon10;
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // run groups for momentum distribution runs
  //
  // isotopes, (center momentum in %, [runset1, 2, ...])
  // isotopes, (center momentum in %, [(left limit, right limit) in mm, ...])
  // isotopes, (center momentum in %, [F1UP PPAC HV, ...])
  static std::map<std::string, std::map<int, std::vector<std::vector<int>>>> mruns;
  static std::vector<int> mdis_o20, mdis_ne24, mdis_all;
  static std::map<std::string, std::map<int, std::vector<std::pair<double, double>>>> mruns_disp;
  static std::map<std::string, std::map<int, std::vector<int>>> mruns_hv;
  static std::map<std::string, std::map<int, std::vector<double>>> mruns_brho;
  ////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////  ////////
  static std::map<std::string, float> brhoValue;
  //////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////
  // for mom dist. analysis
  static void MomDistAnalysis();
  static void NeMomDistAnalysis(bool flagFast = true);
  static void CrossSectionAnalysis();
  ////////////////////////////////////////////////////////
  // new mom dist. analysis functions
  static void MomDistAnalysis(std::string iso);
  static void LoadMomDistAnalysis(std::string iso);
  ////////////////////////////////////////////////////////

  static void PPACEffCurve();

  std::map<std::string, TCutG *> cutgs;
  std::map<std::string, std::map<Int_t, Double_t>> effF1UPPACX;
  std::string allcut;

 private:
  Expt expt;
  std::vector<Int_t> runNs;
  std::vector<THeader *> headers;

  Double_t centBrho;
  Double_t tofOff;
  Double_t pathLength;

  Bool_t useF1;
  Bool_t useF2orF3;    // true = F2, false = F3
  Bool_t useSSDorPla;  // true = SSD, false = Pla

  std::string gcut;

  std::vector<std::string> split(const std::string &s, char delimiter, bool flagEmpty = true);

  ////////////////////////////////////////////////////////
  // tree branches
  int runN;
  TClonesArray *scaler;
  ////////////////////////////////////////////////////////

  ClassDef(KOBRA, 1)
};

#endif
