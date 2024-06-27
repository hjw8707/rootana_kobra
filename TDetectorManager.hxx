#ifndef TDetectorManager_h
#define TDetectorManager_h

#include <string>
#include <map>

#include "TDetectorAna.hxx"
#include "TDataContainer.hxx"

enum DTYPE {
    KoBRATDC,
    KoBRAADC,
    KoBRADIG
};

enum ATYPE {
    PPAC,
    LPPAC,
    Plastic,
    DPlastic,
    SSD
};

class TDetectorManager {

public:
static TDetectorManager* GetInstance();
//inline TDetectorAna* GetDetectorAna() {return Detector;}

void AddDetectorAna(const char* name, DTYPE dtype, const char* bank, TDetectorAna* ana);
void AddDetectorAna(const char* name, DTYPE dtype, const char* bank, ATYPE atype, const char* par, int n = 2);

TDetectorAna* GetDetectorAna(const char* name);
inline std::map<std::string, TDetectorAna*> GetMaps() { return anas; }

void LoadAnaList(const char* filename);


void PrintParametersAll();
void SetTreeAll();
void SetDataAll(TDataContainer &container);
void ClearAll();
void AnalysisAll();

private:
TDetectorManager();
~TDetectorManager();

static TDetectorManager* instance;
std::map<std::string, TDetectorAna*> anas;
std::map<std::string, std::string> banks;
std::map<std::string, DTYPE> dtypes;

std::map<std::string, DTYPE> dtype_map;
std::map<std::string, ATYPE> atype_map;
};

#endif