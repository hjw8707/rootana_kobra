#ifndef THistManager_h
#define THistManager_h

#include "TClonesArray.h"
#include "TTreeFormula.h"
#include "TH1.h"
#include "TH2.h"

#include <string>
#include <map>

class THistManager {

public:
static THistManager* GetInstance();

void AddHistFromFile(const char* file);
void Clear();

inline TH1* Get1DHist(const char* name) { return Hist1D[name]; }
inline TH2* Get2DHist(const char* name) { return Hist2D[name]; }

void Fill();
void Reset();

private:
THistManager();
~THistManager();

Bool_t flagBlock;
static THistManager* instance;

std::map<std::string, TH1*> Hist1D;
std::map<std::string, TH2*> Hist2D;

std::map<std::string, TTreeFormula*> tf1d_x;
std::map<std::string, TTreeFormula*> tf1d_c;

std::map<std::string, TTreeFormula*> tf2d_x;
std::map<std::string, TTreeFormula*> tf2d_y;
std::map<std::string, TTreeFormula*> tf2d_c;
};
#endif