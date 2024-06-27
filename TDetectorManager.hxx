#ifndef TDetectorManager_h
#define TDetectorManager_h

#include <map>

#include "TDetectorAna.hxx"
#include "TDataContainer.hxx"

enum DTYPE {
    KoBRATDC,
    KoBRAADC,
    KoBRADIG
};

class TDetectorManager {

public:
static TDetectorManager* GetInstance();
//inline TDetectorAna* GetDetectorAna() {return Detector;}

void AddDetectorAna(const char* name, TDetectorAna* ana, DTYPE dtype, const char* bank);

TDetectorAna* GetDetectorAna(const char* name);
inline std::map<std::string, TDetectorAna*> GetMaps() { return anas; }


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

};

#endif