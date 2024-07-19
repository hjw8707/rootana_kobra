#ifndef KOBRA_HXX
#define KOBRA_HXX

#include "TObject.h"
#include "TChain.h"
#include "TCutG.h"
#include "TClonesArray.h"
#include "TGraph.h"
#include "TPad.h"


#include <vector>
#include <iostream>
#include <string>

class THeader;

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

  inline void SetBrho(Double_t br)  { centBrho = br; SetAlias(); }
  inline Double_t GetBrho() { return centBrho; }

  inline void SetTOFOffset(Double_t off) { tofOff = off; SetAlias(); }
  inline Double_t GetTOFOffset() { return tofOff; }

  inline void SetUseF1(Bool_t us = true) { useF1 = us; SetAlias(); }
  inline Bool_t GetUseF1() { return useF1; }

  inline void SetUseF2orF3(Bool_t us) { useF2orF3 = us; SetAlias(); }
  inline Bool_t GetUseF2orF3() { return useF2orF3; }

  inline void SetUseF2() { SetUseF2orF3(true); }
  inline void SetUseF3() { SetUseF2orF3(false); }

  inline void SetUseSSDorPla(Bool_t us) { useSSDorPla = us; SetAlias(); }
  inline Bool_t GetUseSSDorPla() { return useSSDorPla; }

  inline void SetUseSSD() { SetUseSSDorPla(true); }
  inline void SetUsePla() { SetUseSSDorPla(false); }
  
  inline void SetZ(Double_t a, Double_t b) { tree->SetAlias("Z", Form("%f*z+%f", a, b)); }
  void SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2);

  inline void SetAoQ(Double_t a, Double_t b) { tree->SetAlias("AoQ", Form("%f*aoq+%f", a, b)); }
  void SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2);

  inline void SetGCut(const char *_c) { gcut = _c; }
  inline const char* GetGCut() { return gcut.c_str(); }

  void DrawPID0(const char *cut = NULL);
  void DrawPID(const char *cut = NULL);
  void DrawPIDC(Int_t show = 0, const char *cut = NULL); // show = 0: nothing, 1: count, 2: pps

  void DrawXDist(const char *cut = NULL);
  void DrawPPACEff(const char *cut = NULL);
  void PrintSetting(std::ostream &out = std::cout);

  void CountIsotopes();  
  
  void AddCut(const char* filename);
  void AddCuts(const char* path);
  
  void DrawAllCut();

  Double_t GetElapsedTime();

  Long64_t GetEntries(const char* selection);

  TGraph* PPACRate();

  void ApplyOffsetToCut(Double_t xoff, Double_t yoff = 0);

  TCutG* Make2DCut(const char* name, const char* title);

  inline THeader* GetHeader(int i) { return headers[i]; }
  
  TChain *tree;
  std::map<Int_t, Double_t> brhoMap;
  std::map<Int_t, Double_t> f1slitMap;

  ////////////////////////////////////////////////////////////
  // run group for each isotope (defined at kobra.cxx)
  static std::vector<int> o18, o19, o20, o21;
  ////////////////////////////////////////////////////////////
  
private:
  std::vector<Int_t> runNs;
  std::vector<THeader*> headers;

  Double_t centBrho;
  Double_t tofOff;

  Bool_t useF1;
  Bool_t useF2orF3; // true = F2, false = F3
  Bool_t useSSDorPla; // true = SSD, false = Pla

  std::string gcut;

  std::vector<std::string> split(const std::string &s, char delimiter);
  std::map<std::string, TCutG*> cutgs;
  
  TClonesArray *ca;


  ClassDef(KOBRA, 1)
};

#endif
