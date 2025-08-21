#ifndef KOBRA_F0_HXX
#define KOBRA_F0_HXX

#include <iostream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TClonesArray.h"
#include "TCut.h"
#include "TCutG.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TObject.h"
#include "TPad.h"

class KOBRAF0 : public TObject {
   public:
    KOBRAF0(const char *file = "misc/f0ratio.csv");
    virtual ~KOBRAF0();

    void LoadCSV(const char *file);
    void PrintCSV();

    double GetRatio(int run);

    static KOBRAF0 *GetInstance(const char *file = "misc/f0ratio.csv");

   private:
    static KOBRAF0 *instance;
    std::vector<int> run0;
    std::vector<int> run1;
    std::vector<double> f0ratio;

    ClassDef(KOBRAF0, 1)
};

std::vector<std::vector<std::string>> parseCSV(const std::string &filename);

#endif
