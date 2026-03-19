#ifndef KOBRABEAM_HXX
#define KOBRABEAM_HXX

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

class KOBRABeam : public TObject {
 public:
  KOBRABeam(const char *file = "misc/beam_time.csv");
  virtual ~KOBRABeam();

  void LoadCSV(const char *file);
  void PrintCSV();

  double GetFloat(int run, const char *col);
  std::string GetDatetime(int run, const char *col);
  std::string GetStartTime(int run);
  std::string GetStopTime(int run);
  double GetElapsedTime(int run);              // in seconds
  double GetMeanLEBTIntensity(int run);        // in A
  double GetIntegralLEBTIntensity(int run);    // in A*s
  double GetIntegralTSLEBTIntensity(int run);  // in A*s using the time stamp values

  double GetMeanLEBT1kIntensity(int run);
  double GetIntegralTSLEBT1kIntensity(int run);

  double GetMeanACCTIntensity(int run);
  double GetMeanACCTIntensityError(int run);  // in ratio
  double GetIntegralTSACCTIntensity(int run);
  double GetIntegralTSACCTIntensityError(int run);  // in ratio

  static KOBRABeam *GetInstance(const char *file = "misc/beam_time.csv");

 private:
  static KOBRABeam *instance;

  std::vector<std::string> cols;
  std::map<int, std::map<std::string, double>> floatMap;
  // datetime 정보를 저장하기 위한 맵 추가
  std::map<int, std::map<std::string, std::string>> datetimeMap;

  ClassDef(KOBRABeam, 1)
};

std::vector<std::vector<std::string>> parseCSV(const std::string &filename);

#endif
