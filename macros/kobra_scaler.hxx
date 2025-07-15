#ifndef KOBRASCALER_HXX
#define KOBRASCALER_HXX

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

class KOBRAScaler : public TObject {
   public:
    KOBRAScaler(const char *file = "misc/scaler.csv");
    virtual ~KOBRAScaler();

    void LoadCSV(const char *file);
    void PrintCSV();

    double Get(int run, const char *col);
    double GetLiveTime(int run);
    double GetLiveTime(std::vector<int> runs);  // average of runs
    double GetTotalTime(int run);
    double GetTotalTime(std::vector<int> runs);  // average of runs

    static KOBRAScaler *GetInstance(const char *file = "misc/scaler.csv");

   private:
    static KOBRAScaler *instance;

    std::vector<std::string> cols;
    std::map<int, std::map<std::string, double>> scaler;

    ClassDef(KOBRAScaler, 1)
};

std::vector<std::vector<std::string>> parseCSV(const std::string &filename);

#endif
