#ifndef THistManager_h
#define THistManager_h

#include "TClonesArray.h"
#include "TTreeFormula.h"
#include "TH1.h"
#include "TH2.h"

#include <string>
#include <vector>

class THistManager {

public:
static THistManager* GetInstance();

void AddHistFromFile(const char* file);
void Clear();

void Fill();
void Reset();

private:
THistManager();
~THistManager();

Bool_t flagBlock;
static THistManager* instance;

std::vector<TH1*> Hist1D;
std::vector<TH2*> Hist2D;

std::vector<TTreeFormula*> tf1d_x;
std::vector<TTreeFormula*> tf1d_c;

std::vector<TTreeFormula*> tf2d_x;
std::vector<TTreeFormula*> tf2d_y;
std::vector<TTreeFormula*> tf2d_c;
};
#endif