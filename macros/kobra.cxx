#include "kobra.hxx"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TChainElement.h"
#include "TColor.h"
#include "TCutG.h"
#include "TEventList.h"
#include "TF2.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TH2.h"
#include "THeader.hxx"
#include "TLatex.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "TScalerData.hxx"
#include "TSelectorEntries.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TVirtualPad.h"
#include "kobra_beam.hxx"
#include "kobra_f0.hxx"
#include "kobra_scaler.hxx"
ClassImp(KOBRA);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for cross section runs
std::vector<int> KOBRA::o18 = {251, 252, 253, 254};
std::vector<int> KOBRA::o19 = {259, 260, 261, 262, 263, 264, 265, 266, 267, 271, 272, 273, 274,
                               275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287,
                               288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299};

std::vector<int> KOBRA::o20 = {304, 305, 306, 307, 308, 309, 310};

std::vector<int> KOBRA::o21 = {316, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331,
                               332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 347};

std::vector<int> KOBRA::o22 = {223, 224, 225, 226,  // Right after the newly adjusted primary beam
                               348, 349, 350, 351, 352, 361, 362, 364, 365, 366, 367, 368, 369, 370, 371,
                               372, 373, 374, 375, 376, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387,
                               388, 389, 390, 391, 392, 393, 394, 395, 396, 398, 399, 400, 402, 403, 404};

std::vector<int> KOBRA::ne24 = {410, 411, 412, 413, 415};

std::vector<int> KOBRA::ne25 = {420, 421, 422};

std::vector<int> KOBRA::ne26 = {425, 426, 427, 428, 429, 430, 431, 433, 434, 435, 436, 437, 438, 439, 440};

std::vector<int> KOBRA::o22bl = {527, 528, 529, 530, 531, 532, 533};
std::vector<int> KOBRA::o21bl = {537, 538, 539, 540, 542};
std::vector<int> KOBRA::o20bl = {546, 547, 548, 549, 550, 551};
std::vector<int> KOBRA::totbl = {553, 554, 555, 556};

std::map<std::string, std::vector<int>> KOBRA::csruns = {
    {"o18", o18},   {"o19", o19},   {"o20", o20},     {"o21", o21},     {"o22", o22},     {"ne24", ne24},
    {"ne25", ne25}, {"ne26", ne26}, {"o22bl", o22bl}, {"o21bl", o21bl}, {"o20bl", o20bl}, {"totbl", totbl}};

std::map<std::string, float> KOBRA::brhoValue = {{"o18", 1.2959},     {"o19", 1.3198},     {"o20", 1.3734},
                                                 {"o21", 1.3879},     {"o22", 1.4600},     {"ne24", 1.2646443},
                                                 {"ne25", 1.3201144}, {"ne26", 1.3741834}, {"o22bl", 1.4600},
                                                 {"o21bl", 1.3879},   {"o20bl", 1.3734},   {"totbl", 1.3734}};

std::vector<std::string> KOBRA::o18_iso = {"c13", "c14", "c15", "n15", "n16", "n17",  "o17",
                                           "o18", "o19", "f19", "f20", "f21", "ne21", "ne22"};
std::vector<std::string> KOBRA::o19_iso = {  // ApplyOffsetToCut(-0.005)
    "c13", "c14", "c15", "c16", "n15", "n16",  "n17",  "n18",  "o17",  "o18", "o19",
    "o20", "f19", "f20", "f21", "f22", "ne21", "ne22", "ne23", "ne24", "na25"};

std::vector<std::string> KOBRA::o20_iso = {  // ApplyOffsetToCut(-0.014,0)
    "c13", "c14", "c15", "c16", "n15", "n16", "n17",  "n18",  "n19",  "o18",  "o19",
    "o20", "o21", "f20", "f21", "f22", "f23", "ne22", "ne23", "ne24", "na25", "na26"};

std::vector<std::string> KOBRA::o21_iso = {  // ApplyOffsetToCut(-0.02,-0.1)
    "c14", "c15", "c16", "c17", "n16", "n17",  "n18",  "n19",  "o18",  "o19",  "o20",  "o21",
    "o22", "f21", "f22", "f23", "f24", "ne23", "ne24", "ne25", "na25", "na26", "na27", "mg28"};

std::vector<std::string> KOBRA::o22_iso = {  // ApplyOffsetToCut(-0.03)
    "c15", "c16", "c17", "n17",  "n18",  "n19",  "o19",  "o20",  "o21", "o22",
    "f22", "f23", "f24", "ne24", "ne25", "ne26", "na27", "na28", "mg29"};

std::vector<std::string> KOBRA::ne24_iso = {  // ApplyOffsetToCut(-0.005)
    "ne24", "na25", "na26", "mg28", "al29", "al30"};

std::vector<std::string> KOBRA::ne25_iso = {  // ApplyOffsetToCut(-0.005, -0.1)
    "ne24", "na25", "na26", "na27", "mg28", "mg29", "al30", "al31"};

std::vector<std::string> KOBRA::ne26_iso = {  // ApplyOffsetToCut(-0.005, -0.1)
    "f22", "f23", "ne24", "na25", "na26", "na27", "mg28", "mg29", "al30", "al31"};

std::map<std::string, std::vector<std::string>> KOBRA::csiso = {
    {"o18", o18_iso}, {"o19", o19_iso},   {"o20", o20_iso},   {"o21", o21_iso},
    {"o22", o22_iso}, {"ne24", ne24_iso}, {"ne25", ne25_iso}, {"ne26", ne26_iso}};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for mom distribution runs
//
// isotopes, (momentum dispersion in %, F1 opening in %, single side), runs
//
// clang-format off
std::map<std::string, std::map<int, std::vector<std::vector<int>>>> KOBRA::mruns = {
    {"o20",{
            {-14, {{656, 657}}},
            {-12, {{643, 644, 645, 646, 647, 648}}},
            {-10, {{639, 640, 641, 642}}},
            { -8, {{620, 621, 622, 623, 625}}},
            { -7, {{651, 652, 653, 655}}},
            { -6, {{615, 616, 617}}},
            { -5, {{660, 661}}},
            { -4, {{610, 611, 612}}},
            {  0, {{601, 602, 603, 604}, {632, 636}, {637, 638}}}, // normal, w/o BD, with BD
            {  4, {{605, 606, 607}}},
            {  7, {{590, 592, 593, 594, 595, 596, 598}}}}},
    {"ne24",{
            {-10, {{775, 776, 777, 778, 779}, {773, 774}}},
            { -8, {{763, 764, 765}, {767, 769}, {770, 771}}},
            { -7, {{780, 781}}},
            { -6, {{754, 755, 756, 757, 758, 759}}},
            { -5, {{782, 783, 784, 785}}},
            { -4, {{750, 751, 752}, {788, 789}}},
            { -3, {{786, 787}}},
            { -2, {{746, 748}}},
            { -1, {{738, 739, 740}}},
            {  0, {{706, 707, 708}}},
            {  1, {{713, 714, 715, 716, 717, 718, 719}}},
            {  2, {{720, 721, 722, 723, 724, 725}}},
            {  4, {{726, 727, 728, 729, 730}}},
            {  6, {{731, 732, 733}}},
            { 10, {{734, 735, 736, 737}}}}}};

std::map<std::string, std::map<int, std::vector<std::pair<double, double>>>> KOBRA::mruns_disp = {
    {"o20", {
            {-14, {{-2, 0}}},
            {-12, {{-5, 1}}},
            {-10, {{-6.5, 2.5}}},
            { -8, {{-23, 19}}},
            { -7, {{-35, 31}}},
            { -6, {{-25, 21}}},
            { -5, {{-55, 51}}},
            { -4, {{-45, 41}}},
            {  0, {{-190, 190}, {-190, 190}, {-190, 190}}},
            {  4, {{-190, 190}}},
            {  7, {{-190, 190}}}}},
    {"ne24", {
            {-10, {{-1, 0}, {-6.1, 2.1}}},
            { -8, {{-2, 0}, {2, 0}, {-6.1, 2.1}}},
            { -7, {{-4, 0}}},
            { -6, {{-4.5, 0.5}}},
            { -5, {{-5.5, 1.5}}},
            { -4, {{-6.2, 2.2}, {-6.2, 2.2}}},
            { -3, {{-7, 3}}},
            { -2, {{-11, 7}}},
            { -1, {{-19, 15}}},
            {  0, {{-25, 21}}},
            {  1, {{-37, 33}}},
            {  2, {{-49, 45}}},
            {  4, {{-86, 82}}},
            {  6, {{-128, 124}}},
            { 10, {{-128, 124}}}}}};

std::map<std::string, std::map<int, std::vector<int>>> KOBRA::mruns_hv = {
    {"o20", {
            {-14, {550}},
            {-12, {550}},
            {-10, {550}},
            { -8, {550}},
            { -7, {550}},
            { -6, {575}},
            { -5, {550}},
            { -4, {585}},
            {  0, {595, 595, 595}},
            {  4, {595}},
            {  7, {595}}}},
    {"ne24", {
            {-10, {550, 0}},
            { -8, {550, 0, 0}},
            { -7, {550}},
            { -6, {550}},
            { -5, {550}},
            { -4, {550, 550}},
            { -3, {550}},
            { -2, {550}},
            { -1, {550}},
            {  0, {550}},
            {  1, {550}},
            {  2, {550}},
            {  4, {550}},
            {  6, {550}},
            { 10, {550}}}}};

std::map<std::string, std::map<int, std::vector<double>>> KOBRA::mruns_brho = {
    {"o20", {
            {-14, {1.2064 * 0.9600000}},
            {-12, {1.2384 * 0.9590333}},
            {-10, {1.2666 * 0.9632790}},
            { -8, {1.2947 * 0.9665260}},
            { -7, {1.3088 * 0.9671648}},
            { -6, {1.3299 * 0.9642969}},
            { -5, {1.3369 * 0.9701535}},
            { -4, {1.3510 * 0.9702033}},
            {  0, {1.4073 * 0.9758002, 1.4087 * 0.975682, 1.4087 * 0.975409}},
            {  4, {1.4636 * 0.9795254}},
            {  7, {1.5065 * 0.9816280}}}},
    {"ne24", {
            {-10, {1.1768, 1.1768}},
            { -8, {1.2030, 1.2030, 1.2030}},
            { -7, {1.2161}},
            { -6, {1.2291}},
            { -5, {1.2422}},
            { -4, {1.2553, 1.2553}},
            { -3, {1.2684}},
            { -2, {1.2814}},
            { -1, {1.2945}},
            {  0, {1.3076}},
            {  1, {1.3207}},
            {  2, {1.3337}},
            {  4, {1.3599}},
            {  6, {1.3861}},
            { 10, {1.4384}}}}};

// clang-format on
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for mom distribution runs
std::vector<int> KOBRA::mom_14 = {656, 657};                          // ApplyOffsetToCut(0.04)
std::vector<int> KOBRA::mom_12 = {643, 644, 645, 646, 647, 648};      // ApplyOffsetToCut(0.04)
std::vector<int> KOBRA::mom_10 = {639, 640, 641, 642};                // ApplyOffsetToCut(0.035)
std::vector<int> KOBRA::mom_08 = {620, 621, 622, 623, 625};           // ApplyOffsetToCut(0.025)
std::vector<int> KOBRA::mom_07 = {651, 652, 653, 655};                // ApplyOffsetToCut(0.025)
std::vector<int> KOBRA::mom_06 = {615, 616, 617};                     // ApplyOffsetToCut(0.02, -0.1)
std::vector<int> KOBRA::mom_05 = {660, 661};                          // ApplyOffsetToCut(0.015, -0.1)
std::vector<int> KOBRA::mom_04 = {610, 611, 612};                     // ApplyOffsetToCut(0.015, -0.1)
std::vector<int> KOBRA::mom00 = {601, 602, 603, 604};                 // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mom04 = {605, 606, 607};                      // ApplyOffsetToCut(-0.005,0.1)
std::vector<int> KOBRA::mom07 = {590, 592, 593, 594, 595, 596, 598};  // ApplyOffsetToCut(-0.01,0.2)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for mom distribution runs
std::vector<int> KOBRA::mon_10 = {775, 776, 777, 778, 779};  // ApplyOffsetToCut(0.05)
std::vector<int> KOBRA::mon_08 = {770, 771};                 // ppac no
std::vector<int> KOBRA::monn_08 = {763, 764, 765};
std::vector<int> KOBRA::mon_07 = {780, 781};  // ApplyOffsetToCut(0.032)
std::vector<int> KOBRA::mon_06 = {754, 755, 756, 757, 758, 759};
std::vector<int> KOBRA::mon_05 = {782, 783, 784, 785};
std::vector<int> KOBRA::mon_04 = {788, 789, 750, 751, 752};           // ApplyOffsetToCut(?)
std::vector<int> KOBRA::monn_04 = {750, 751, 752};                    // ApplyOffsetToCut(?)
std::vector<int> KOBRA::mon_03 = {786, 787};                          // ApplyOffsetToCut(0.005)
std::vector<int> KOBRA::mon_02 = {746, 748};                          // ApplyOffsetToCut(0.005)
std::vector<int> KOBRA::mon_01 = {738, 739, 740};                     // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mon00 = {706, 707, 708};                      // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mon01 = {713, 714, 715, 716, 717, 718, 719};  // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon02 = {720, 721, 722, 723, 724, 725};       // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon04 = {726, 727, 728, 729, 730};            // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon06 = {731, 732, 733};                      // ApplyOffsetToCut(-0.01)
std::vector<int> KOBRA::mon10 = {734, 735, 736, 737};                 // ApplyOffsetToCut(-0.02)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

KOBRA::KOBRA(Expt expt) : expt(expt) { Initilize(); }

KOBRA::KOBRA(Expt expt, const char *file) : expt(expt) {
  Initilize();
  LoadTree(file);
}
KOBRA::KOBRA(Expt expt, std::string file) : expt(expt) {
  Initilize();
  LoadTree(file);
}
KOBRA::KOBRA(Expt expt, std::vector<std::string> files) : expt(expt) {
  Initilize();
  LoadTree(files);
}
KOBRA::KOBRA(Expt expt, int run) : expt(expt) {
  Initilize();
  LoadTree(expt, run);
  runNs.push_back(run);
  RunSetting(run);
}
KOBRA::KOBRA(Expt expt, std::vector<int> runs) : expt(expt) {
  Initilize();
  LoadTree(expt, runs);
  for (const auto &it : runs) runNs.push_back(it);
  RunSetting(runs[0]);
}
KOBRA::KOBRA(Expt expt, std::vector<int> runs, float brho) : expt(expt) {
  Initilize();
  LoadTree(expt, runs);
  for (const auto &it : runs) runNs.push_back(it);
  RunSetting(runs[0]);
  SetBrho(brho);
}
KOBRA::KOBRA(Expt expt, int runb, int rune) : expt(expt) {
  Initilize();
  LoadTree(expt, runb, rune);
  for (int i = runb; i <= rune; i++) runNs.push_back(i);
  RunSetting(runb);
}

KOBRA::KOBRA(Expt expt, std::map<int, std::vector<std::vector<int>>> runmap) : expt(expt) {
  Initilize();
  std::vector<int> runs;
  for (const auto &it : runmap) {
    for (const auto &rung : it.second) {
      for (const auto &run : rung) {
        runs.push_back(run);
      }
    }
  }
  LoadTree(expt, runs);
  for (const auto &it : runs) runNs.push_back(it);
  RunSetting(runs.front());
}

KOBRA::~KOBRA() {
  if (tree) delete tree;
}

void KOBRA::Initilize() {
  tree = new TChain("kobra", "kobra");
  centBrho = 1.5065;
  tofOff = 0;
  pathLength = 12398.5;
  useF1 = false;
  useF2orF3 = false;
  useSSDorPla = true;
  // gcut = "f3ssd@.GetEntriesFast() > 0 && f3ssd@.GetEntriesFast() < 3";
  gcut = "";
  allcut = "";

  LoadDB("macros/runlog.txt");
  gStyle->SetPadGridX(true);
  gStyle->SetPadGridY(true);

  gStyle->SetCanvasDefX(100);
  gStyle->SetCanvasDefY(100);
  //  gStyle->SetPalette(kPastel);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
  Double_t red[NRGBs] = {0.00, 0.00, 0.87, 1.00, 0.51};
  Double_t green[NRGBs] = {0.00, 0.81, 1.00, 0.20, 0.00};
  Double_t blue[NRGBs] = {0.51, 1.00, 0.12, 0.00, 0.00};
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  //////////////////////////////////////////////////////////////
  // F1 Up PPACX Efficiency Table
  std::map<Int_t, Double_t> eff_b = {{595, 0.59661017}};
  std::map<Int_t, Double_t> eff_c = {{595, 0.82855191}};
  std::map<Int_t, Double_t> eff_n = {{550, 0.842323}, {575, 0.92880259}, {585, 0.75492958}, {595, 0.90255720}};
  std::map<Int_t, Double_t> eff_o = {{550, 0.73289565}, {575, 0.91565041}, {585, 0.85066247}, {595, 0.95522972}};
  std::map<Int_t, Double_t> eff_f = {{550, 0.56216995}, {575, 0.87934893}, {585, 0.89564770}, {595, 0.95945083}};
  std::map<Int_t, Double_t> eff_ne = {{550, 0.40854047}, {575, 0.82282681}, {585, 0.93298634}, {595, 0.96422389}};
  std::map<Int_t, Double_t> eff_na = {{550, 0.25688073}, {575, 0.72759227}, {585, 0.94747082}, {595, 0.96993988}};
  std::map<Int_t, Double_t> eff_mg = {{550, 0.91540785}};
  effF1UPPACX["b"] = eff_b;
  effF1UPPACX["c"] = eff_c;
  effF1UPPACX["n"] = eff_n;
  effF1UPPACX["o"] = eff_o;
  effF1UPPACX["f"] = eff_f;
  effF1UPPACX["ne"] = eff_ne;
  effF1UPPACX["na"] = eff_na;
  effF1UPPACX["mg"] = eff_mg;
  //////////////////////////////////////////////////////////////
}

Bool_t KOBRA::LoadTree(const char *filename) {
  // Check if file exists
  if (gSystem->AccessPathName(filename)) {
    std::cout << "File " << filename << " does not exist." << std::endl;
    return true;
  }

  // Open the ROOT file
  TFile file(filename, "READ");

  // Check if the file is valid
  if (!file.IsOpen() || file.IsZombie()) {
    std::cout << "File " << filename << " is not a valid ROOT file." << std::endl;
    return false;
  }

  // Check if the TTree exists in the file
  TTree *tr = dynamic_cast<TTree *>(file.Get("kobra"));
  if (!tr) {
    std::cout << "TTree kobra does not exist in file " << filename << "." << std::endl;
    return false;
  }

  THeader *head = dynamic_cast<THeader *>(file.Get("header"));
  if (!head) {
    std::cout << "THeader does not exist in file " << filename << "." << std::endl;
    return false;
  }

  // Add the file to the TChain
  tree->Add(filename);
  headers.push_back(head);
  std::cout << "TTree kobra from file " << filename << " added to TChain." << std::endl;
  return true;
}

Bool_t KOBRA::LoadTree(std::string file) { return LoadTree(file.c_str()); }

Bool_t KOBRA::LoadTree(std::vector<std::string> files) {
  Bool_t result = true;
  for (const auto &it : files) {
    result = result && LoadTree(it.c_str());
  }
  return result;
}

Bool_t KOBRA::LoadTree(Expt expt, int run) {
  switch (expt) {
    case Expt::Comm:
      return LoadTree(Form("root/comm/output%08d.root", run));
    case Expt::Phys:
      return LoadTree(Form("root/phys/output%08d.root", run));
  }
}

Bool_t KOBRA::LoadTree(Expt expt, std::vector<int> runs) {
  Bool_t result = true;
  for (const auto &it : runs) {
    result = result && LoadTree(expt, it);
  }
  return result;
}

Bool_t KOBRA::LoadTree(Expt expt, int runb, int rune) {
  if (rune < runb) return false;

  Bool_t result = true;
  for (int run = runb; run <= rune; run++) {
    result = result && LoadTree(expt, run);
  }
  return result;
}

std::vector<std::string> KOBRA::split(const std::string &s, char delimiter, bool flagEmpty) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    if (flagEmpty || !token.empty()) tokens.push_back(token);
  }
  return tokens;
}

Bool_t KOBRA::LoadDB(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    // #으로 시작하는 주석을 제거
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) line = line.substr(0, commentPos);

    // 공백만 있는 줄은 무시
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos) continue;

    std::vector<std::string> tokens = split(line, '\t');
    Int_t run;
    try {
      run = std::stoi(tokens[0]);
    } catch (...) {
      continue;
    }

    Double_t brho;
    try {
      brho = std::stof(tokens[7]);
    } catch (...) {
      continue;
    }
    brhoMap[run] = brho;

    Double_t f1slit;
    try {
      f1slit = std::stof(tokens[11]) + std::stof(tokens[12]);
    } catch (...) {
      continue;
    }
    f1slitMap[run] = f1slit;
  }

  file.close();
  return true;
}

void KOBRA::RunSetting(int run) {
  try {
    SetBrho(brhoMap[run]);
  } catch (...) {
  }
  // SetBrho(1.356);
  SetUseF1(true);
  // AddCuts("./cut");
  if (run >= 588)
    SetUsePla();
  else
    SetUseSSD();

  scaler = NULL;
  tree->SetBranchAddress("scaler", &scaler);
  tree->SetBranchAddress("runN", &runN);
}

void KOBRA::SetPathLength(Double_t len) {
  if (len > 0) {
    tree->SetAlias("beta", Form("%f/tof/299.", len));
    pathLength = len;
  } else {
    tree->SetAlias("beta", Form("12398.5/tof/299."));
    pathLength = 12398.5;
  }
}

void KOBRA::SetStyleAhn() {
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
}

void KOBRA::SetAlias() {
  if (!tree) return;

  tree->SetAlias("f1a", "(f1dppac.x - f1uppac.x)/527*1000.");
  tree->SetAlias("f1b", "(f1dppac.y - f1uppac.y)/527*1000.");
  tree->SetAlias("f2a", "(f2dppac.x - f2uppac.x)/480*1000.");
  tree->SetAlias("f2b", "(f2dppac.y - f2uppac.y)/480*1000.");
  tree->SetAlias("f3a", "(f3dppac.x - f3uppac.x)/570*1000.");
  tree->SetAlias("f3b", "(f3dppac.y - f3uppac.y)/570*1000.");
  tree->SetAlias("f1x", "f1uppacx.x");
  tree->SetAlias("f1y", "f1uppac.y");
  tree->SetAlias("f2y", "f2dppac.y");
  tree->SetAlias("f2x", "f2dppac.x");
  tree->SetAlias("f2y", "f2dppac.y");
  tree->SetAlias("f3x", "760.*(f3a/1000.)+f3dppac.x");
  tree->SetAlias("f3y", "760.*(f3b/1000.)+f3dppac.y");
  tree->SetAlias("f3ssdx", "160.*(f3a/1000.)+f3uppac.x");
  tree->SetAlias("f3ssdy", "160.*(f3b/1000.)+f3uppac.y");
  tree->SetAlias("f3plax", "310.*(f3a/1000.)+f3uppac.x");
  tree->SetAlias("f3play", "310.*(f3b/1000.)+f3uppac.y");

  tree->SetAlias("f2t_raw", "f2dppt.tl");
  tree->SetAlias("f2t", "f2dppt.tsum");
  tree->SetAlias("f3t_raw", "f3uppt.tl");
  tree->SetAlias("f3t", "f3uppt.tsum");

  tree->SetAlias("db", "f1uppacx.x[0]");  // only choose the first candidate (avoid confusing from multiple counts)
  tree->SetAlias("tof", Form("(f3uppt.tsum[0] - f2dppt.tsum[0]) + 103.8 + %f",
                             tofOff));  // only choose the first candidate (avoid confusing from multiple counts)
  tree->SetAlias("tofp", Form("(f3pla.GetTAve()[0] - f2dppt.tsum[0]) + 103.8 - 6.62 + %f",
                              tofOff));  // only choose the first candidate (avoid confusing from multiple counts)
  //  tree->SetAlias("de", "Max$(f2ssd.acal)");
  tree->SetAlias("ycor", "(-0.00245304*f3uppac.y[0]^2-0.034376*f3uppac.y[0]+19.8065)/20");
  tree->SetAlias("ycord", "(-0.743174*f3uppac.y[0]^2+5.14862*f3uppac.y[0]+3216.66)/3000");
  // de
  tree->SetAlias("depla", "sqrt(f3dpla.al[0]*f3dpla.ar[0])/ycord");
  tree->SetAlias("dessd2", "Max$(f2ssd.acal)");
  tree->SetAlias("dessd3", "Max$(f3ssd.acal)/ycor");
  tree->SetAlias("de", useSSDorPla ? (useF2orF3 ? "dessd2" : "dessd3") : "depla");

  tree->SetAlias("brho", Form("(1+db/4100)*%f", centBrho));

  tree->SetAlias("beta", "12398.5/tof/299.");  // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gamma", "1/sqrt(1-beta*beta)");
  tree->SetAlias("pp", "gamma*beta");

  tree->SetAlias("betap", "12398.5/tofp/299.");  // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gammap", "1/sqrt(1-betap*betap)");
  tree->SetAlias("ppp", "gammap*betap");

  tree->SetAlias("aoq", useF1 ? "brho/(3.1*pp)" : Form("%f/(3.1*pp)", centBrho));
  tree->SetAlias("aoqp", useF1 ? "brho/(3.1*ppp)" : Form("%f/(3.1*ppp)", centBrho));

  tree->SetAlias("AoQ", "0.941841*aoq+0.095597");
  tree->SetAlias("AoQp", "0.941841*aoqp+0.095597");

  tree->SetAlias("corfac", "log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
  tree->SetAlias("corfac_pla", "log(2*511./0.0647*beta*beta/gamma/gamma) - beta*beta");

  tree->SetAlias("z_ssd3", "sqrt(dessd3)*beta/sqrt(corfac)");
  tree->SetAlias("z_ssd2", "sqrt(dessd2)*beta/sqrt(corfac)");
  tree->SetAlias("z_pla", "sqrt(depla)*beta/sqrt(corfac_pla)");

  tree->SetAlias("Z_ssd3", "21.540118*z_ssd3+0.444610");
  tree->SetAlias("Z_ssd2", "21.540118*z_ssd2+0.444610");
  tree->SetAlias("Z_pla", "40.1885*z_pla-1.51331");

  tree->SetAlias("z", useSSDorPla ? (useF2orF3 ? "z_ssd2" : "z_ssd3") : "z_pla");
  tree->SetAlias("Z", useSSDorPla ? (useF2orF3 ? "Z_ssd2" : "Z_ssd3") : "Z_pla");

  ////////////////////////////////////////////
  // de-tof cut
  tree->SetAlias("detof", "de-0.12*(tof-230)");
  tree->SetAlias("detof_f", "abs(detof - 25) < 2.1");
  tree->SetAlias("detof_o", "abs(detof - 20) < 2.5");
  tree->SetAlias("detof_n", "abs(detof - 15) < 2.");
  tree->SetAlias("detof_c", "abs(detof - 11) < 2.");
  ////////////////////////////////////////////
}

void KOBRA::AddCut(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
    return;
  }

  int linen = 0;
  std::string line;
  TCutG *cut;
  while (std::getline(file, line)) {
    // #으로 시작하는 주석을 제거
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) line = line.substr(0, commentPos);

    // 공백만 있는 줄은 무시
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos) continue;

    std::vector<std::string> tokens = split(line, ' ', false);
    if (linen == 0) {
      cut = new TCutG(tokens[0].c_str());
      if (tokens.size() > 1) cut->SetTitle(tokens[1].c_str());
    } else {
      try {
        cut->AddPoint(std::stof(tokens[0]), std::stof(tokens[1]));
      } catch (...) {
        continue;
      }
    }
    linen++;
  }
  if (cut) {
    cut->SetVarX("aoq");
    cut->SetVarY("Z");
    cut->SetFillStyle(1000);
    cut->SetLineStyle(2);
    cutgs[cut->GetName()] = cut;
  }
  file.close();
}

void KOBRA::AddCuts(const char *path) {
  for (const auto it : cutgs) {
    delete (it.second);
  }
  cutgs.clear();
  TSystemDirectory dir(path, path);

  TList *filesList = dir.GetListOfFiles();
  if (!filesList) {
    std::cerr << "Error: " << path << " is not a valid directory or cannot be accessed.\n";
    return;
  }

  TIter next(filesList);
  TSystemFile *file;
  while ((file = (TSystemFile *)next())) {
    TString fileName = file->GetName();
    if (!file->IsDirectory() && fileName.EndsWith("txt")) {
      AddCut(Form("%s/%s", path, fileName.Data()));
    }
  }

  SetCutStyle(2, 2);

  ////////////////////////////////////////////////////////////
  // add a cut for all isotopes
  for (const auto &it : cutgs) {
    if (!allcut.empty()) {
      allcut += "||";
    }
    allcut += it.second->GetName();
  }
  ////////////////////////////////////////////////////////////
}

Long64_t KOBRA::Draw(const char *name, const char *cut, const char *option) {
  if (!tree) return 0;
  return tree->Draw(name, cut, option);
}

TH2 *KOBRA::DrawPID0(const char *cut) {
  if (!tree) return nullptr;

  std::string cuts = gcut;
  if (cut) {
    if (cuts.empty()) {
      cuts += Form("%s", cut);
    } else {
      cuts += Form("&& %s", cut);
    }
  }
  tree->Draw("de:tof>>hpid0(400,150,350,400,0,150)", cuts.c_str(), "col");
  return static_cast<TH2 *>(gDirectory->Get("hpid0"));
}

TH2 *KOBRA::DrawPID(Int_t show, const char *cut) {
  if (show < 0 || show > 2) return nullptr;
  if (!tree) return nullptr;

  std::string cuts = gcut;
  if (cut) {
    if (cuts.empty()) {
      cuts += Form("%s", cut);
    } else {
      cuts += Form("&& %s", cut);
    }
  }
  auto c1 = new TCanvas(Form("cpid_r%d", runNs.front()), "PID", 1600, 1200);
  tree->Draw(Form("Z:aoq>>hpid_r%d(500,2.0,3.2,500,4,15)", runNs.front()), cuts.c_str(), "col");

  std::string runstr;
  if (runNs.size() > 1)
    runstr = Form("Run %d-%d", runNs.front(), runNs.back());
  else
    runstr = Form("Run %d", runNs.front());

  const char *showcase[3] = {"", ": Count", ": Rate [pps]"};
  auto h2 = static_cast<TH2 *>(gDirectory->Get(Form("hpid_r%d", runNs.front())));
  h2->SetTitle(Form("PID for %s%s;A/Q;Z", runstr.c_str(), showcase[show]));

  if (show > 0) {
    TLatex text;
    text.SetTextFont(62);
    text.SetTextSize(0.03);
    text.SetTextColor();
    Double_t time = GetElapsedTime();
    for (const auto &it : cutgs) {
      TCutG *cut = it.second;
      // cut->SetLineStyle(2);
      cut->Draw("SAME");

      Double_t x = cut->GetMean(1);
      Double_t y = cut->GetMean(2);
      Double_t xmin, xmax, ymin, ymax;
      cut->ComputeRange(xmin, ymin, xmax, ymax);
      TString title = cut->GetTitle();
      TString aaa = title(0, 2);
      TString zzz = title(2, title.Length());
      Int_t count = GetEntries(cut->GetName());
      text.SetTextFont(62);
      text.SetTextColor(kOrange + 7);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      text.DrawLatex(x, ymax, Form("^{%s}%s", aaa.Data(), zzz.Data()));

      text.SetTextFont(52);
      text.SetTextColor(kMagenta + 1);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      TString a;

      if (show == 1)
        a = Form("%d", count);
      else
        a = Form("%.3f", float(count / time));

      text.DrawLatex(x, y, a.Data());
    }
    DrawBrhoTimeInfo(gPad, GetBrho(), time);
  }
  return h2;
}

TH2 *KOBRA::DrawPIDC(Int_t show, const char *cut) {
  if (show < 0 || show > 2) return nullptr;
  if (!tree) return nullptr;

  std::string cuts = gcut;
  if (cut) {
    if (cuts.empty()) {
      cuts += Form("%s", cut);
    } else {
      cuts += Form("&& %s", cut);
    }
  }
  auto c1 = new TCanvas(Form("cpid_r%d", runNs.front()), "PID", 1600, 1200);
  tree->Draw(Form("Z:AoQ>>hpidc_r%d(500,2.0,3.2,500,4,15)", runNs.front()), cuts.c_str(), "col");

  TH2 *h2 = static_cast<TH2 *>(gDirectory->Get(Form("hpidc_r%d", runNs.front())));

  std::string runstr;
  if (runNs.size() > 1)
    runstr = Form("Run %d-%d", runNs.front(), runNs.back());
  else
    runstr = Form("Run %d", runNs.front());

  const char *showcase[3] = {"", ": Count", ": Rate [pps]"};
  h2->SetTitle(Form("PID for %s%s;A/Q;Z", runstr.c_str(), showcase[show]));

  if (show > 0) {
    TLatex text;
    text.SetTextFont(62);
    text.SetTextSize(0.03);
    text.SetTextColor();
    Double_t time = GetElapsedTime();
    for (const auto &it : cutgs) {
      TCutG *cut = it.second;
      // cut->SetLineStyle(2);
      cut->Draw("SAME");

      Double_t x = cut->GetMean(1);
      Double_t y = cut->GetMean(2);
      Double_t xmin, xmax, ymin, ymax;
      cut->ComputeRange(xmin, ymin, xmax, ymax);
      TString title = cut->GetTitle();
      TString aaa = title(0, 2);
      TString zzz = title(2, title.Length());
      Int_t count = GetEntries(cut->GetName());
      text.SetTextFont(62);
      text.SetTextColor(kOrange + 7);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      text.DrawLatex(xmax, ymax, Form("^{%s}%s", aaa.Data(), zzz.Data()));

      text.SetTextFont(52);
      text.SetTextColor(kMagenta + 1);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      TString a;

      if (show == 1)
        a = Form("%d", count);
      else
        a = Form("%.3f", float(count / time));

      text.DrawLatex(x, y, a.Data());
    }
    DrawBrhoTimeInfo(gPad, GetBrho(), time);
  }
  return h2;
}

void KOBRA::SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2) {
  Double_t a = static_cast<Double_t>(iz2 - iz1) / (z2 - z1);
  Double_t b = static_cast<Double_t>(iz1) - a * z1;
  SetZ(a, b);
}

void KOBRA::SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2) {
  Double_t a = static_cast<Double_t>(iz2 - iz1) / (z2 - z1);
  Double_t b = static_cast<Double_t>(iz1) - a * z1;
  SetAoQ(a, b);
}

void KOBRA::PrintSetting(std::ostream &out) {
  out << "=====================================" << std::endl;
  out << "        KOBRA: Current Setting" << std::endl;
  out << "=====================================" << std::endl;
  out << "  Run Number  : " << runNs.front() << std::endl;
  out << "  Brho Setting: " << GetBrho() << " Tm" << std::endl;
  out << "  Use F1      : " << (GetUseF1() ? "O" : "X") << std::endl;
  out << "  Use F2 / F3 : " << (GetUseF2orF3() ? "F2" : "F3") << std::endl;
  out << "=====================================" << std::endl;
  out << "  TOF Offset  : " << GetTOFOffset() << " ns" << std::endl;
  out << "  Global Cut  : " << GetGCut() << std::endl;
  out << "=====================================" << std::endl;
}

void KOBRA::CountIsotopes(std::ostream &out, bool flagTitle) {
  std::stringstream ss;
  ////////////////////////////////////////////////////////////
  // Counting
  if (flagTitle) {
    ss << "=========================================" << std::endl;
    ss << "   Counts of Isotopes " << std::endl;
    ss << "=========================================" << std::endl;
  }
  for (const auto &it : cutgs) {
    ss << " ";
    ss << it.second->GetTitle();
    ss << ": ";
    ss << GetEntries(it.second->GetName());
    ss << std::endl;
  }
  if (flagTitle) {
    ss << "=========================================" << std::endl;
  }
  ////////////////////////////////////////////////////////////
  out << ss.str();
}

void KOBRA::RateIsotopes(std::ostream &out, bool flagTitle) {
  Double_t time = GetElapsedTime();
  std::stringstream ss;

  ////////////////////////////////////////////////////////////
  // Counting
  if (flagTitle) {
    ss << "=========================================" << std::endl;
    ss << "   Rates of Isotopes " << std::endl;
    ss << "=========================================" << std::endl;
  }
  for (const auto &it : cutgs) {
    ss << " ";
    ss << it.second->GetTitle();
    ss << ": ";
    ss << static_cast<double>(GetEntries(it.second->GetName()) / time);
    ss << std::endl;
  }
  if (flagTitle) {
    ss << "=========================================" << std::endl;
    ////////////////////////////////////////////////////////////
    out << ss.str();
  }
}

void KOBRA::CountIsotopesRunByRun(std::ostream &out, bool flagTitle, bool flagHeader) {
  std::stringstream ss;
  ////////////////////////////////////////////////////////////
  // Counting
  if (flagTitle) {
    ss << "===================================================================================================="
          "="
          "======================"
       << std::endl;
    ss << "   Counts of Isotopes Run by Run" << std::endl;
    ss << "===================================================================================================="
          "="
          "======================"
       << std::endl;
  }
  if (flagHeader) {
    ss << "Run | ";
    for (const auto &it : cutgs) {
      ss << it.second->GetTitle();
      ss << " | ";
    }
    ss << std::endl;
  }
  for (const auto &run : runNs) {
    ss << run << " | ";
    tree->SetEventList(nullptr);
    tree->Draw(Form(">>elist_%d", run), Form("runN==%d", run));
    auto elist = static_cast<TEventList *>(gDirectory->Get(Form("elist_%d", run)));
    tree->SetEventList(elist);
    for (const auto &it : cutgs) {
      ss << tree->GetEntries(it.second->GetName()) << " | ";
    }
    ss << std::endl;
    delete elist;
  }
  if (flagTitle) {
    ss << "===================================================================================================="
          "="
          "======================"
       << std::endl;
  }
  ////////////////////////////////////////////////////////////
  out << ss.str();
}

void KOBRA::DrawCut(const char *cut, bool flagName) {
  TCutG *cutg = cutgs[cut];
  if (!cutg) return;
  cutg->Draw("SAME");
  if (flagName) {
    TLatex text;
    text.SetTextFont(62);
    text.SetTextSize(0.02);
    text.SetTextColor(kViolet);
    text.DrawLatex(cutg->GetMean(1), cutg->GetMean(2), cutg->GetTitle());
  }
}

void KOBRA::DrawAllCuts(bool flagName) {
  for (const auto &it : cutgs) {
    DrawCut(it.second->GetName(), flagName);
  }
}

std::vector<TH1 *> KOBRA::DrawXDist(const char *cut, bool flagDraw) {
  TString cutname;
  if (cut)
    cutname = cut;
  else
    cutname = "nocut";

  TString drawopt;
  TCanvas *c1;
  if (flagDraw) {
    c1 = new TCanvas(Form("cxd_r%d_%s", runNs.front(), cutname.Data()), "X Distribution", 1600, 400);
    c1->Divide(4, 1);
  } else
    drawopt = "goff";

  std::vector<TH1 *> hists;

  if (flagDraw) c1->cd(1);
  tree->Draw(Form("f1x>>hf1x_r%d_%s(100,-200,200)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf1x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw) c1->cd(2);
  tree->Draw(Form("f2x>>hf2x_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf2x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw) c1->cd(3);
  tree->Draw(Form("f3x>>hf3x_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf3x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw) c1->cd(4);
  tree->Draw(Form("f3uppac.x>>hf3ux_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf3ux_r%d_%s", runNs.front(), cutname.Data()))));

  return hists;
}

TH1 *KOBRA::DrawMomDist(const char *cut, Bool_t flagRate, Bool_t flagDraw, Int_t nBin, Double_t lowLim,
                        Double_t highLim) {
  std::string cutname = cut ? cut : "nocut";
  gStyle->SetOptStat(0);
  TString drawOpt;
  if (flagDraw) {
    auto c1 = new TCanvas(Form("cmd_r%d_%s", runNs.front(), cutname.c_str()), "Mom. Distribution", 1200, 800);
    drawOpt = "text0";
  } else
    drawOpt = "goff";

  tree->Draw(Form("f1x>>hmd_r%d_%s(%d,%f,%f)", runNs.front(), cutname.c_str(), nBin, lowLim, highLim), cut,
             drawOpt.Data());

  gStyle->SetPaintTextFormat();
  auto h1 = static_cast<TH1 *>(gDirectory->Get(Form("hmd_r%d_%s", runNs.front(), cutname.c_str())));
  Double_t time = KOBRAScaler::GetInstance()->GetTotalTime(runNs);
  if (flagRate) {
    gStyle->SetPaintTextFormat(".3f");
    h1->Scale(1 / time);
  }
  std::string runstr;
  if (runNs.size() > 1)
    runstr = Form("Run %d-%d", runNs.front(), runNs.back());
  else
    runstr = Form("Run %d", runNs.front());

  h1->SetTitle(Form("F1 X for %s;F1 X [mm];", runstr.c_str()));

  h1->SetMarkerSize(2);

  if (flagDraw) DrawBrhoTimeInfo(gPad, GetBrho(), time);
  return h1;
}

void KOBRA::DrawBrhoTimeInfo(TVirtualPad *pad, Double_t brho, Double_t time) {
  TLatex text;
  text.SetTextFont(62);
  text.SetTextSize(0.03);
  text.SetTextColor();

  std::string timestr;
  Int_t itime = time;
  if (itime > 3600)
    timestr = Form("%02d:%02d:%02d", itime / 3600, (itime % 3600) / 60, itime % 60);
  else if (itime > 60)
    timestr = Form("%02d:%02d", itime / 60, itime % 60);
  else
    timestr = "";

  TVirtualPad *prev_pad = gPad;
  pad->cd();
  text.DrawLatexNDC(0.15, 0.92, Form("Brho: %.4f Tm / Time: %.0f sec. or %s", brho, time, timestr.c_str()));
  prev_pad->cd();
}

TGraphErrors *KOBRA::GetMomDistGraph(Double_t center, const char *cut, Double_t momBinSize, Double_t momLowLim,
                                     Double_t momUppLim, Double_t scale, Bool_t flagUseF1) {
  const Double_t momDispersion = 41;  // mm / %
  const Double_t momCenter = -2;      // mm = 0%
  ////////////////////////////////////////////////////////
  // Set the bin size and the range for DrawMomDist
  Int_t nBin = TMath::Nint((momUppLim - momLowLim) / momBinSize);
  Double_t lowLim = momLowLim * momDispersion + momCenter;
  Double_t highLim = momUppLim * momDispersion + momCenter;
  ////////////////////////////////////////////////////////
  auto h1 = DrawMomDist(cut, true, false, nBin, lowLim, highLim);

  TGraphErrors *gr = new TGraphErrors;
  gr->SetTitle(Form("Momentum Distribution of %s;Momentum Dispersion (%%);Rate per Mom. %% (pps/%%)", cut));

  if (flagUseF1) {
    for (Int_t i = 1; i <= h1->GetNbinsX(); i++) {
      Double_t curPoint = (h1->GetBinCenter(i) - momCenter) / momDispersion;  // displaced by momCenter
      if (curPoint < momLowLim || curPoint > momUppLim) continue;
      gr->AddPoint(curPoint + center, h1->GetBinContent(i) * scale / momBinSize);
      gr->SetPointError(gr->GetN() - 1, momBinSize * 0.5, h1->GetBinError(i) * scale / momBinSize);
    }
  } else {  // just a single bin when we are not using F1 PPAC
    Double_t time = KOBRAScaler::GetInstance()->GetTotalTime(runNs);
    Int_t event = tree->GetEntries(cut);
    gr->AddPoint(center, event / time * scale / momBinSize);
    gr->SetPointError(0, 0.05, TMath::Sqrt(event) / time * scale / momBinSize);
  }
  gr->SetMarkerStyle(20);
  gr->SetMarkerColor(1);
  gr->SetMarkerSize(2);
  return gr;
}

std::map<std::string, TGraphErrors *> KOBRA::GetMomDistGraphs(std::vector<std::string> iso, Double_t center,
                                                              Double_t f1SlitLowLim, Double_t f1SlitUppLim, Int_t bias,
                                                              Bool_t flagUseF1) {
  ////////////////////////////////////////////////////////
  // scale factor
  //
  // F2, F3 PPAC efficiency, Live time -> Corrected inversely (all they are less than 1)
  //
  Double_t scale = 1. / (GetPPACEff(2) * GetPPACEff(3) * KOBRAScaler::GetInstance()->GetLiveTime(runNs));
  //
  // F0 ratio -> Corrected inversely (less than 1)
  scale *= (1. / KOBRAF0::GetInstance()->GetRatio(runNs.front()));
  //
  // beam intensity -> corrected inversely (cross section is proportional to yield / beam intensity)
  scale *= (1. / KOBRABeam::GetInstance()->GetMeanBeamIntensity(runNs.front()));
  //
  // Others
  //  Time: already corrected in DrawMomDist (if using F1) or GetMomDistGraph (if not using F1)
  //  F1 PPAC efficiency: corrected in GetMomDistGraph (if using F1), no correction is necessary if not using F1
  ////////////////////////////////////////////////////////
  // more scale factor
  const Double_t momDispersion = 41;  // mm / %
  const Double_t momCenter = -2;      // mm = 0%

  ////////////////////////////////////////////////////////
  // Set the bin size and the range for DrawMomDist
  // Default size and range for the momentum distribution (-166 mm ~ +162 mm, with 20.5 mm bin size)
  // This setting is available for the full open slit (-190 ~ 190)
  Double_t momBinSize = 0.5;  // %
  ////////////////////////////////////////////////////////
  // if the slit is not full open, we need to set the bin size and the range for DrawMomDist
  Double_t momLowLim = std::ceil((f1SlitLowLim - momCenter) / momDispersion / (0.5 * momBinSize)) * (0.5 * momBinSize);
  Double_t momUppLim = std::floor((f1SlitUppLim - momCenter) / momDispersion / (0.5 * momBinSize)) * (0.5 * momBinSize);
  // The maximum and minimum limits for the momentum distribution
  if (momLowLim < -3) momLowLim = -3;  // -166 mm -> -125 mm
  if (momUppLim > 3) momUppLim = 3;    // +162 mm -> +121 mm
  // if the slit is narrower than 0.5%, we need to set the bin size and the range for DrawMomDist
  if (momLowLim > -0.25) {
    // try 0.1 bin size
    momBinSize = 0.1;
    momLowLim = std::ceil((f1SlitLowLim - momCenter) / momDispersion / (0.5 * momBinSize)) * (0.5 * momBinSize);
    momUppLim = std::floor((f1SlitUppLim - momCenter) / momDispersion / (0.5 * momBinSize)) * (0.5 * momBinSize);
    if (momLowLim > -0.05) {  // only this case for (-2, 0) -> asymmetric slit
      momBinSize = 0.05;      // 0.05 %
      momLowLim = 0;          // 0 %
      momUppLim = 0.05;       // 0.05 %
    }
  }
  ////////////////////////////////////////////////////////
  std::map<std::string, TGraphErrors *> momgraphs;
  for (const auto it : iso) {
    TGraphErrors *gr;
    if (flagUseF1)
      gr = GetMomDistGraph(center, it.c_str(), momBinSize, momLowLim, momUppLim,
                           scale / GetPPACEff(1, NULL, true, it, bias));
    else
      gr = GetMomDistGraph(center, it.c_str(), momBinSize, momLowLim, momUppLim, scale, flagUseF1);
    momgraphs[it] = gr;
  }
  return momgraphs;
}

Double_t KOBRA::GetElapsedTime() {
  std::vector<Long64_t> nEntries;

  TObjArray *fileElements = tree->GetListOfFiles();
  for (TObject *op : *fileElements) {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile f{chainElement->GetTitle()};
    TTree *tree = f.Get<TTree>(chainElement->GetName());
    nEntries.push_back(tree->GetEntries());
  }

  std::vector<Long64_t> nSum;
  for (auto it = nEntries.begin(); it != nEntries.end(); it++) {
    Int_t sum = std::accumulate(nEntries.begin(), it + 1, 0);
    nSum.push_back(sum);
  }

  Double_t totalTime = 0;
  for (auto it = nSum.begin(); it != nSum.end(); it++) {
    Int_t n0, n1;
    if (it == nSum.begin()) {
      for (int i = 0; i <= tree->GetEntries(); i++) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n0 = static_cast<TScalerData *>(scaler->At(0))->reft;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n1 = static_cast<TScalerData *>(scaler->At(0))->reft;
          break;
        }
      }
    } else {
      for (int i = *(it - 1); i <= tree->GetEntries(); i++) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n0 = static_cast<TScalerData *>(scaler->At(0))->reft;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n1 = static_cast<TScalerData *>(scaler->At(0))->reft;
          break;
        }
      }
    }
    totalTime += (Double_t)(n1 - n0);
  }
  return totalTime;
}

Long64_t KOBRA::GetEntries(const char *selection) {
  TSelectorEntries *s = new TSelectorEntries(selection);
  tree->Process(s);
  tree->SetNotify(nullptr);
  Long64_t res = s->GetSelectedRows();
  delete s;
  return res;
}

void KOBRA::DrawPPACTimings(bool flagLog, const char *cut) {
  auto c1 = new TCanvas(Form("cptim_r%d", runNs.front()), "PPAC Timings", 1200, 800);

  const int nppacs = 4;
  const int nquants = 5;

  c1->Divide(nquants, nppacs);

  const char *ppacs[nppacs] = {"f1uppacx", "f2dppac", "f3uppac", "f3dppac"};
  const char *quants[nquants] = {"tx1", "tx2", "ty1", "ty2", "ta"};

  TH1 *h1;
  for (Int_t i = 0; i < nppacs; i++) {
    for (Int_t j = 0; j < nquants; j++) {
      TVirtualPad *vpad = c1->cd(i * nquants + j + 1);
      tree->Draw(Form("%s.%s>>h%s_%s_r%d(1000,0,100000)", ppacs[i], quants[j], ppacs[i], quants[j], runNs.front()),
                 cut);
      vpad->SetLogy(flagLog);
      auto h1 = static_cast<TH1 *>(gDirectory->Get(Form("h%s_%s_r%d", ppacs[i], quants[j], runNs.front())));
      h1->GetXaxis()->SetRange(h1->FindFirstBinAbove(0), h1->FindLastBinAbove(0));
      h1->Draw();
    }
  }
}

void KOBRA::DrawPPACEff(const char *cut) {
  auto c1 = new TCanvas(Form("cpeff_r%d", runNs.front()), "PPAC Eff.", 800, 1200);
  c1->Divide(2, 3);

  TH1 *h1;
  c1->cd(1);
  tree->Draw(Form("f1uppac@.GetEntriesFast()>>hf1up_r%d(2,0,2)", runNs.front()), cut);
  h1 = static_cast<TH1 *>(gDirectory->Get(Form("hf1up_r%d", runNs.front())));
  std::cout << " F1 Up PPAC Eff. : " << h1->GetMean() << std::endl;

  c1->cd(2);
  tree->Draw(Form("f1uppacx@.GetEntriesFast()>>hf1upx_r%d(2,0,2)", runNs.front()), cut);
  h1 = static_cast<TH1 *>(gDirectory->Get(Form("hf1upx_r%d", runNs.front())));
  std::cout << " F1 Up PPACX Eff.: " << h1->GetMean() << std::endl;

  c1->cd(4);
  tree->Draw(Form("f2dppac@.GetEntriesFast()>>hf2dp_r%d(2,0,2)", runNs.front()), cut);
  h1 = static_cast<TH1 *>(gDirectory->Get(Form("hf2dp_r%d", runNs.front())));
  std::cout << " F2 Dn PPAC Eff. : " << h1->GetMean() << std::endl;

  c1->cd(5);
  tree->Draw(Form("f3uppac@.GetEntriesFast()>>hf3up_r%d(2,0,2)", runNs.front()), cut);
  h1 = static_cast<TH1 *>(gDirectory->Get(Form("hf3up_r%d", runNs.front())));
  std::cout << " F3 Up PPAC Eff. : " << h1->GetMean() << std::endl;

  c1->cd(6);
  tree->Draw(Form("f3dppac@.GetEntriesFast()>>hf3dp_r%d(2,0,2)", runNs.front()), cut);
  h1 = static_cast<TH1 *>(gDirectory->Get(Form("hf3dp_r%d", runNs.front())));
  std::cout << " F3 Dn PPAC Eff. : " << h1->GetMean() << std::endl;
}

TGraph *KOBRA::PPACRate(int gap) {
  TGraph *gr = new TGraph;

  Double_t lastts = -1;
  Double_t lastppac = -1;
  for (Int_t i = 0; i < tree->GetEntries(); i += gap) {
    tree->GetEntry(i);
    if (!scaler || scaler->GetEntriesFast() == 0) continue;
    time_t start_ts = GetHeader(tree->GetTreeNumber())->GetStartTimeStamp();
    time_t day0 = start_ts - (start_ts % 86400) - 9 * 3600;  // 00:00:00 of the day
    if (i > 1) {
      Double_t dt = (double)(static_cast<TScalerData *>(scaler->At(0))->ts - lastts);
      Double_t dppac = (double)(static_cast<TScalerData *>(scaler->At(0))->ppac - lastppac);
      if (dt != 0) {
        gr->AddPoint(day0 + static_cast<TScalerData *>(scaler->At(0))->reft, dppac / (dt * 1.E-8));
      }
    }
    lastts = (double)(static_cast<TScalerData *>(scaler->At(0))->ts);
    lastppac = (double)(static_cast<TScalerData *>(scaler->At(0))->ppac);
  }

  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  gr->GetXaxis()->SetNdivisions(505);

  gr->SetTitle("PPAC Or Rate;Time [s];Rate [pps]");
  gr->Draw("AL");
  return gr;
}

TGraph *KOBRA::TriggerRate(int gap) {
  TGraph *gr = new TGraph;

  Double_t lastts = -1;
  Double_t lasttrig = -1;
  for (Int_t i = 0; i < tree->GetEntries(); i += gap) {
    tree->GetEntry(i);
    if (!scaler || scaler->GetEntriesFast() == 0) continue;
    time_t start_ts = GetHeader(tree->GetTreeNumber())->GetStartTimeStamp();
    time_t day0 = start_ts - (start_ts % 86400) - 9 * 3600;  // 00:00:00 of the day
    if (i > 1) {
      Double_t dt = (double)(static_cast<TScalerData *>(scaler->At(0))->ts - lastts);
      Double_t dtrig = (double)(static_cast<TScalerData *>(scaler->At(0))->hwtriga - lasttrig);
      if (dt != 0) {
        gr->AddPoint(day0 + static_cast<TScalerData *>(scaler->At(0))->reft, dtrig / (dt * 1.E-8));
      }
    }
    lastts = (double)(static_cast<TScalerData *>(scaler->At(0))->ts);
    lasttrig = (double)(static_cast<TScalerData *>(scaler->At(0))->hwtriga);
  }

  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  gr->GetXaxis()->SetNdivisions(505);

  gr->SetTitle("Trigger Rate;Time [s];Rate [pps]");
  gr->Draw("AL");
  return gr;
}

void KOBRA::ApplyOffsetToCut(Double_t xoff, Double_t yoff, Double_t xscale, Double_t yscale) {
  for (const auto &it : cutgs) {
    TCutG *cut = it.second;
    for (Int_t i = 0; i < cut->GetN(); i++) {
      cut->GetX()[i] += xoff;
      cut->GetY()[i] += yoff;
    }
    // Scaling
    Double_t xmean = cut->GetMean(1);
    Double_t ymean = cut->GetMean(2);
    for (Int_t i = 0; i < cut->GetN(); i++) {
      if (xscale > 0) cut->GetX()[i] = xscale * (cut->GetX()[i] - xmean) + xmean;
      if (yscale > 0) cut->GetY()[i] = yscale * (cut->GetY()[i] - ymean) + ymean;
    }
  }
}

void KOBRA::SetCutStyle(Int_t color, Int_t width, Int_t style) {
  for (const auto &it : cutgs) {
    TCutG *cut = it.second;
    cut->SetLineColor(color);
    cut->SetLineWidth(width);
    cut->SetLineStyle(style);
  }
}

Double_t KOBRA::GetPPACEff(Int_t id, const char *cut, bool flagTable, std::string iso, int bias) {
  if (flagTable) {
    auto eff = effF1UPPACX[iso][bias];
    if (eff > 0) return eff;
  }
  TString ccut = "de > 0";
  if (cut) {
    ccut += " && ";
    ccut += cut;
  }

  TH1 *h1;
  switch (id) {
    case 1:
      tree->Draw("f1uppacx@.GetEntriesFast() > 0>>hf1uppacxeff(2,0,2)", ccut.Data(), "goff");
      h1 = static_cast<TH1 *>(gDirectory->Get("hf1uppacxeff"));
      return h1->GetMean();
      break;
    case 2:
      tree->Draw("f2dppac@.GetEntriesFast() > 0>>hf2dppaceff(2,0,2)", ccut.Data(), "goff");
      h1 = static_cast<TH1 *>(gDirectory->Get("hf2dppaceff"));
      return h1->GetMean();
      break;
    case 3:
      tree->Draw("f3uppac@.GetEntriesFast() > 0>>hf3uppaceff(2,0,2)", ccut.Data(), "goff");
      h1 = static_cast<TH1 *>(gDirectory->Get("hf3uppaceff"));
      return h1->GetMean();
      break;
    default:
      return 0;
  }
}

Double_t KOBRA::GetLiveTime() {
  std::vector<Long64_t> nEntries;

  TObjArray *fileElements = tree->GetListOfFiles();
  for (TObject *op : *fileElements) {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile f{chainElement->GetTitle()};
    TTree *tree = f.Get<TTree>(chainElement->GetName());
    nEntries.push_back(tree->GetEntries());
  }

  std::vector<Long64_t> nSum;
  for (auto it = nEntries.begin(); it != nEntries.end(); it++) {
    Int_t sum = std::accumulate(nEntries.begin(), it + 1, 0);
    nSum.push_back(sum);
  }

  Int_t totalUngated = 0;
  Int_t totalGated = 0;
  for (auto it = nSum.begin(); it != nSum.end(); it++) {
    Int_t n0un, n1un, n0ga, n1ga;
    if (it == nSum.begin()) {
      for (int i = 0; i <= tree->GetEntries(); i++) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n0un = static_cast<TScalerData *>(scaler->At(0))->hwtrigb;
          n0ga = static_cast<TScalerData *>(scaler->At(0))->hwtriga;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n1un = static_cast<TScalerData *>(scaler->At(0))->hwtrigb;
          n1ga = static_cast<TScalerData *>(scaler->At(0))->hwtriga;
          break;
        }
      }
    } else {
      for (int i = *(it - 1); i <= tree->GetEntries(); i++) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n0un = static_cast<TScalerData *>(scaler->At(0))->hwtrigb;
          n0ga = static_cast<TScalerData *>(scaler->At(0))->hwtriga;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--) {
        tree->GetEntry(i);
        if (scaler->GetEntriesFast() > 0) {
          n1un = static_cast<TScalerData *>(scaler->At(0))->hwtrigb;
          n1ga = static_cast<TScalerData *>(scaler->At(0))->hwtriga;
          break;
        }
      }
    }
    totalUngated += (n1un - n0un);
    totalGated += (n1ga - n0ga);
  }
  ////////////////////////////////////////////////////////////////
  // Reference pulse subtraction
  totalGated -= tree->GetEntries("scaler.refp > 0");
  ////////////////////////////////////////////////////////////////
  return static_cast<Double_t>((Double_t)totalGated / totalUngated);
}

void KOBRA::AnaAtOnce(const char *name, std::vector<std::string> iso) {
  Double_t livetime;
  Double_t eff1, eff2, eff3;

  std::vector<int> yield;

  //////////////////////////////////////////////////////////////
  // PID plot
  //////////////////////////////////////////////////////////////
  DrawPIDC(1);
  gPad->GetCanvas()->Update();
  gPad->GetCanvas()->Print(Form("analysis/%s/pid.pdf", name));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // Live Time, PPAC eff
  //////////////////////////////////////////////////////////////
  livetime = GetLiveTime();
  eff1 = GetPPACEff(1);
  eff2 = GetPPACEff(2);
  eff3 = GetPPACEff(3);
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // Yield
  //////////////////////////////////////////////////////////////
  for (const auto &it : iso) yield.push_back(tree->GetEntries(it.c_str()));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // X dist.
  //////////////////////////////////////////////////////////////
  std::vector<std::vector<TH1 *>> allhists;
  std::vector<std::vector<Double_t>> allmeans;
  std::vector<std::vector<Double_t>> allsigmas;
  for (const auto &it : iso) {
    allhists.push_back(DrawXDist(it.c_str(), false));
    std::vector<Double_t> means;
    std::vector<Double_t> sigmas;
    for (const auto &it2 : allhists.back()) {
      means.push_back(it2->GetMean());
      sigmas.push_back(it2->GetStdDev());
    }
    allmeans.push_back(means);
    allsigmas.push_back(sigmas);
  }

  // PDF saving
  TCanvas *cxdist = new TCanvas("cxdist", "X Dist", 1600, 2400);
  cxdist->Divide(4, 6);

  int i = 0;
  for (const auto &it : allhists) {
    for (const auto &itt : it) {
      cxdist->cd((i % 24) + 1);
      itt->Draw();
      i++;
    }
    if (i % 24 == 0) {
      cxdist->Print(Form("analysis/%s/xdist.pdf%s", name, i == 24 ? "(" : ""));
      cxdist->Clear("D");
    }
  }
  cxdist->Print(Form("analysis/%s/xdist.pdf%s", name, i < 24 ? "" : ")"));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // text saving
  //////////////////////////////////////////////////////////////
  std::stringstream ss;

  ss << "LiveTime   \t F1PPAC Eff \t F2PPAC Eff \t F3PPAC Eff\n";
  ss << Form("%8.5f \t %8.5f \t %8.5f \t %8.5f \n", livetime, eff1, eff2, eff3);
  ss << "\n";
  ss << "Isotopes   \t Yield \t F1 Mean \t F1 Sigma \t F2 Mean \t F2 Sigma \t F3 Mean \t F3 Sigma \n";
  for (size_t i = 0; i < iso.size(); i++) {
    ss << Form("%8s \t", iso[i].c_str());
    ss << Form("%8lld \t", tree->GetEntries(iso[i].c_str()));
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][0], allsigmas[i][0]);
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][1], allsigmas[i][1]);
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][3], allsigmas[i][3]);
    ss << "\n";
  }

  std::ofstream fout(Form("analysis/%s/result.txt", name));
  std::cout << ss.str();
  fout << ss.str();
  fout.close();
  //////////////////////////////////////////////////////////////
}

void KOBRA::PrintCutCoord(TCutG *cut, Double_t xoff, Double_t yoff) {
  if (!cut) return;

  std::cout << Form("%s %s\n", cut->GetName(), cut->GetTitle());
  for (int i = 0; i < cut->GetN(); i++) {
    std::cout << Form("%8.5f %8.5f\n", cut->GetX()[i] + xoff, cut->GetY()[i] + yoff);
  }
}

void KOBRA::AnaAtOnceMom(const char *name, Double_t delta, std::vector<std::string> iso) {
  Double_t livetime;
  Double_t eff1, eff2, eff3;

  std::vector<int> yield;
  if (iso.empty())
    for (const auto it : cutgs) iso.push_back(it.first);

  //////////////////////////////////////////////////////////////
  // PID plot
  //////////////////////////////////////////////////////////////
  DrawPIDC(1);
  gPad->GetCanvas()->Update();
  gPad->GetCanvas()->Print(Form("analysis/%s/pid.pdf", name));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // Live Time, PPAC eff
  //////////////////////////////////////////////////////////////
  livetime = GetLiveTime();
  eff1 = GetPPACEff(1);
  eff2 = GetPPACEff(2);
  eff3 = GetPPACEff(3);
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // Yield
  //////////////////////////////////////////////////////////////
  for (const auto &it : iso) yield.push_back(tree->GetEntries(it.c_str()));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // X dist.
  //////////////////////////////////////////////////////////////
  std::vector<std::vector<TH1 *>> allhists;
  std::vector<std::vector<Double_t>> allmeans;
  std::vector<std::vector<Double_t>> allsigmas;
  for (const auto &it : iso) {
    allhists.push_back(DrawXDist(it.c_str(), false));
    std::vector<Double_t> means;
    std::vector<Double_t> sigmas;
    for (const auto &it2 : allhists.back()) {
      means.push_back(it2->GetMean());
      sigmas.push_back(it2->GetStdDev());
    }
    allmeans.push_back(means);
    allsigmas.push_back(sigmas);
  }

  // PDF saving
  TCanvas *cxdist = new TCanvas("cxdist", "X Dist", 1600, 2400);
  cxdist->Divide(4, 6);

  int i = 0;
  for (const auto &it : allhists) {
    for (const auto &itt : it) {
      cxdist->cd((i % 24) + 1);
      itt->Draw();
      i++;
    }
    if (i % 24 == 0) {
      cxdist->Print(Form("analysis/%s/xdist.pdf%s", name, i == 24 ? "(" : ""));
      cxdist->Clear("D");
    }
  }
  cxdist->Print(Form("analysis/%s/xdist.pdf%s", name, i < 24 ? "" : ")"));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // text saving
  //////////////////////////////////////////////////////////////
  std::stringstream ss;

  ss << "LiveTime   \t F1PPAC Eff \t F2PPAC Eff \t F3PPAC Eff\n";
  ss << Form("%8.5f \t %8.5f \t %8.5f \t %8.5f \n", livetime, eff1, eff2, eff3);
  ss << "\n";
  ss << "Isotopes   \t Yield \t F1 Mean \t F1 Sigma \t F2 Mean \t F2 Sigma \t F3 Mean \t F3 Sigma \n";
  for (size_t i = 0; i < iso.size(); i++) {
    ss << Form("%8s \t", iso[i].c_str());
    ss << Form("%8lld \t", tree->GetEntries(iso[i].c_str()));
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][0], allsigmas[i][0]);
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][1], allsigmas[i][1]);
    ss << Form("%8.5f \t %8.5f \t", allmeans[i][3], allsigmas[i][3]);
    ss << "\n";
  }

  std::ofstream fout(Form("analysis/%s/result.txt", name));
  std::cout << ss.str();
  fout << ss.str();
  fout.close();
  //////////////////////////////////////////////////////////////
}

void KOBRA::CrossSectionAnalysis() {
  std::vector<std::vector<int>> runs = {o18, o19, o20, o21, o22};
  std::vector<std::string> runnames = {"o18", "o19", "o20", "o21", "o22"};
  std::vector<Double_t> xoff = {
      0, -0.005, -0.014, -0.02, -0.03,
  };
  std::vector<Double_t> yoff = {0, 0, 0, -0.1, 0};
  std::vector<std::vector<std::string>> isos = {o18_iso, o19_iso, o20_iso, o21_iso, o22_iso};

  KOBRA *ko;
  for (size_t j = 0; j < runs.size(); j++) {
    ko = new KOBRA(Expt::Phys, runs[j]);
    ko->ApplyOffsetToCut(xoff[j], yoff[j]);
    ko->AnaAtOnce(runnames[j].c_str(), isos[j]);
    delete ko;
  }
}

void KOBRA::PPACEffCurve() {
  std::vector<Int_t> runs = {162, 163, 164, 165, 166, 167};
  std::vector<Double_t> bias = {565, 575, 585, 595, 600, 605};
  std::vector<Double_t> eff_c, eff_n, eff_o, eff_f;

  for (const auto &it : runs) {
    KOBRA ko(Expt::Phys, it);
    eff_c.push_back(ko.GetPPACEff(1, "detof_c"));
    eff_n.push_back(ko.GetPPACEff(1, "detof_n"));
    eff_o.push_back(ko.GetPPACEff(1, "detof_o"));
    eff_f.push_back(ko.GetPPACEff(1, "detof_f"));
  }

  TGraph *gr_c = new TGraph(bias.size(), bias.data(), eff_c.data());
  TGraph *gr_n = new TGraph(bias.size(), bias.data(), eff_n.data());
  TGraph *gr_o = new TGraph(bias.size(), bias.data(), eff_o.data());
  TGraph *gr_f = new TGraph(bias.size(), bias.data(), eff_f.data());

  gr_c->SetMarkerColor(1);
  gr_c->SetMarkerStyle(20);
  gr_c->SetLineColor(1);
  gr_n->SetMarkerColor(2);
  gr_n->SetMarkerStyle(21);
  gr_n->SetLineColor(2);
  gr_o->SetMarkerColor(3);
  gr_o->SetMarkerStyle(22);
  gr_o->SetLineColor(3);
  gr_f->SetMarkerColor(4);
  gr_f->SetMarkerStyle(23);
  gr_f->SetLineColor(4);

  TMultiGraph *mgr = new TMultiGraph("f1uppac_eff", "F1 Up PPAC Eff.");
  mgr->Add(gr_c);
  mgr->Add(gr_n);
  mgr->Add(gr_o);
  mgr->Add(gr_f);

  mgr->Draw("APC");
}

void KOBRA::NeMomDistAnalysis(bool flagFast) {
  KOBRA *ko;
  ///////////////////////////////////////////////////////////////////
  // TMultiGraphs definitions
  std::vector<TMultiGraph *> mgs;
  std::vector<std::string> cutiso = {"o20",  "o21",  "f21",  "f22",  "f23",  "ne23",
                                     "ne24", "ne25", "na25", "na26", "mg27", "mg28"};

  for (const auto it : cutiso) {
    auto mg =
        new TMultiGraph(it.c_str(), Form("Mom. Distribution of %s;Delta [%%];Differental Rate [pps/%%]", it.c_str()));
    mgs.push_back(mg);
  }
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // Loop for all settings
  std::vector<std::vector<int>> runs = {mon_10, monn_08, mon_07, mon_06, mon_05, mon_04, mon_03, mon_02,
                                        mon_01, mon00,   mon01,  mon02,  mon04,  mon06,  mon10};

  std::vector<std::string> runnames = {"mon_10", "monn_08", "mon_07", "mon_06", "mon_05", "mon_04", "mon_03", "mon_02",
                                       "mon_01", "mon00",   "mon01",  "mon02",  "mon04",  "mon06",  "mon10"};
  std::vector<Int_t> bias = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<Double_t> xoff = {0.050, 0.037, 0.032,  0.027,  0.025,  0.025, 0.012, 0.005,
                                0,     0.,    -0.005, -0.005, -0.005, -0.01, -0.02};
  std::vector<Double_t> yoff = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<Double_t> brho = {1.1768, 1.2030, 1.2161, 1.2291, 1.2422, 1.2553, 1.2684, 1.2814,
                                1.2945, 1.3076, 1.3207, 1.3337, 1.3599, 1.3861, 1.4384};
  std::vector<Double_t> cens = {-10, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 4, 6, 10};
  std::vector<Double_t> bins = {0.025, 0.05, 0.1, 0.1, 0.15, 0.2, 0.2, 0.4, 0.4, 0.5, 0.4, 0.5, 0.5, 0.5, 0.5};
  std::vector<Double_t> lefts = {-0.005, -0.01, -0.04, -0.04, -0.1, -0.1, -0.1, -0.2, -0.4, -0.5, -0.5, -1, -2, -3, -3};
  std::vector<Double_t> rights = {0.02, 0.04, 0.04, 0.04, 0.1, 0.1, 0.1, 0.2, 0.4, 0.5, 0.5, 1, 2, 3, 3};
  std::vector<bool> offs = {false, false, true,  true,  true,  true,  true, true,
                            false, false, false, false, false, false, false};
  //  std::vector<bool> usef1 = {true, true, true, true,true,true,true,true,true,true,true,true,true };

  for (size_t j = 0; j < runs.size(); j++) {
    //      if (j == 2) continue;
    ko = new KOBRA(Expt::Phys, runs[j]);
    ko->SetBrho(brho[j]);
    ko->SetUseF1(true);
    ko->ApplyOffsetToCut(xoff[j], yoff[j]);
    if (!flagFast) ko->AnaAtOnceMom(runnames[j].c_str(), cens[j], cutiso);
    // auto grs = ko->GetMomDistGraphsNe(cutiso, cens[j], bins[j], lefts[j], rights[j], offs[j], bias[j], true);
    // for (size_t i = 0; i < mgs.size(); i++) {
    //     // size_t i = 25;
    //     grs[i]->SetMarkerStyle(20 + j);
    //     grs[i]->SetMarkerColor(j + 1);
    //     grs[i]->SetMarkerSize(0.7);
    //     mgs[i]->Add(grs[i]);
    // }
    delete ko;
  }
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Drawing
  // PDF saving
  TCanvas *cxdist = new TCanvas("cmdist", "Mom Dist", 1200, 1600);
  cxdist->Divide(3, 4);

  const int nhist = 12;
  int i = 0;
  for (const auto &it : mgs) {
    cxdist->cd((i % nhist) + 1);
    it->Draw("AP");
    i++;
    if (i % nhist == 0) {
      //      cxdist->Print(Form("analysis/nemomdist.pdf%s", i == nhist ? "(" : ""));
      // cxdist->Clear("D");
    }
  }
  cxdist->Print(Form("analysis/nemomdist.pdf%s", i < nhist ? "" : ")"));
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Root file saving
  TFile *file = new TFile("analysis/nemomdist.root", "RECREATE");
  for (const auto &it : mgs) {
    it->Write();
  }
  file->Close();
  delete file;
  /////////////////////////////////////////////////////////////////////
}

void KOBRA::FitPIDLocus(const char *name, int nbinsX, int nbinsY) {
  tree->Draw(Form("Z:aoq>>hpidlocus(%d,1.5,3,%d,0,20)", nbinsX, nbinsY), name);
  TH2 *h2 = static_cast<TH2 *>(gDirectory->Get("hpidlocus"));
  Fit2DGaussian(h2);
}

void KOBRA::Fit2DGaussian(TH2 *hist) {
  // Create 1D projections of the histogram
  TH1D *projX = hist->ProjectionX("projX");
  TH1D *projY = hist->ProjectionY("projY");

  projX->Fit("gaus", "Q0");
  TF1 *f1 = projX->GetFunction("gaus");
  // Get the initial parameters from the 1D fits
  Double_t meanX = f1->GetParameter(1);
  Double_t sigmaX = f1->GetParameter(2);

  projY->Fit("gaus", "Q0");
  f1 = projY->GetFunction("gaus");
  Double_t meanY = f1->GetParameter(1);
  Double_t sigmaY = f1->GetParameter(2);

  // Clean up
  delete projX;
  delete projY;

  // Create a 2D Gaussian function
  TF2 *gaussian =
      new TF2("gaussian", "[0]*exp(-0.5*((x-[1])/[2])**2)*exp(-0.5*((y-[3])/[4])**2)", hist->GetXaxis()->GetXmin(),
              hist->GetXaxis()->GetXmax(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());

  // Set initial parameters for the Gaussian
  gaussian->SetParameters(1, meanX, sigmaX, meanY, sigmaY);

  // Fit the histogram with the Gaussian function
  hist->Fit(gaussian, "Q0");

  // Optionally, draw the fitted function on the histogram
  gaussian->Draw("SAME");

  std::cout << "Fitted Gaussian parameters: ";
  std::cout << "Amplitude = " << gaussian->GetParameter(0) << ", ";
  std::cout << "Mean X = " << gaussian->GetParameter(1) << ", ";
  std::cout << "Sigma X = " << gaussian->GetParameter(2) << ", ";
  std::cout << "Mean Y = " << gaussian->GetParameter(3) << ", ";
  std::cout << "Sigma Y = " << gaussian->GetParameter(4) << std::endl;
}

void KOBRA::ExtractScalerInfo(bool flagPrint, const char *filename) {
  std::stringstream ss;
  ss << "run, ts, ppac, f2pl, f3pl, hwtriga, hwtrigb, refp, reft" << std::endl;
  for (int i = 0; i <= tree->GetEntries(); i++) {
    tree->GetEntry(i);
    if (scaler->GetEntriesFast() > 0) {
      ss << runN << ", ";
      static_cast<TScalerData *>(scaler->At(0))->PrintSingleLine(ss);
      ss << std::endl;
    }
  }
  if (flagPrint) std::cout << ss.str();
  if (filename) {
    std::ofstream fout(filename);
    fout << ss.str();
    fout.close();
  }
}

void Fit2DGaussian(TH2 *hist, const char *cut) {
  // Create a 2D Gaussian function
  TF2 *gaussian = new TF2("gaussian", "gaus(0)*gaus(2)", hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                          hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());

  // Set initial parameters for the Gaussian
  gaussian->SetParameters(1, hist->GetMean(1), hist->GetMean(2), hist->GetRMS(1), hist->GetRMS(2));

  // Fit the histogram with the Gaussian function
  hist->Fit(gaussian, "RQ0");

  // Optionally, draw the fitted function on the histogram
  gaussian->Draw("SAME");
}

void KOBRA::MomDistAnalysis(std::string iso) {
  KOBRA *ko;

  std::map<int, std::vector<std::vector<int>>> mruns_selected = mruns[iso];
  std::map<int, std::vector<std::pair<double, double>>> mruns_disp_selected = mruns_disp[iso];
  std::map<int, std::vector<int>> mruns_hv_selected = mruns_hv[iso];
  std::map<int, std::vector<double>> mruns_brho_selected = mruns_brho[iso];

  std::vector<std::string> cutiso;
  std::vector<std::string> cutiso_o20 = {"f21", "f22", "f23", "o19", "o20", "o21", "n17", "n18", "n19"};
  std::vector<std::string> cutiso_ne24 = {"o20",  "o21",  "f21",  "f22",  "f23",  "ne23",
                                          "ne24", "ne25", "na25", "na26", "mg27", "mg28"};

  if (iso == "o20") {
    cutiso = cutiso_o20;
  } else if (iso == "ne24") {
    cutiso = cutiso_ne24;
  } else {
    std::cout << "Invalid iso: " << iso << std::endl;
  }
  ///////////////////////////////////////////////////////////////////
  // TMultiGraphs definitions
  std::map<std::string, TMultiGraph *> mgs;
  for (const auto it : cutiso) {
    auto mg =
        new TMultiGraph(it.c_str(), Form("Mom. Distribution of %s;Delta [%%];Differental Rate [pps/%%]", it.c_str()));
    mgs[it] = mg;
  }
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // Loop for all settings to get the mom dist graphs
  int counter = 0;
  for (const auto &it : mruns_selected) {
    int mom_center = it.first;
    std::vector<std::vector<int>> runs = it.second;
    for (size_t i = 0; i < runs.size(); i++) {
      if (i > 0) break;
      KOBRA *ko = new KOBRA(Expt::Phys, runs[i], mruns_brho_selected[mom_center][i]);
      auto grs = ko->GetMomDistGraphs(cutiso, float(mom_center), mruns_disp_selected[mom_center][i].first,
                                      mruns_disp_selected[mom_center][i].second, mruns_hv_selected[mom_center][i]);
      for (const auto &it : cutiso) {
        grs[it]->SetMarkerStyle(20 + counter);
        grs[it]->SetMarkerColor(counter + 1);
        grs[it]->SetMarkerSize(0.7);
        grs[it]->SetName(Form("momdist_%s_%d_%zu", iso.c_str(), mom_center, i));
        mgs[it]->Add(grs[it]);
      }
      delete ko;
    }
    counter++;
  }
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Drawing
  // PDF saving
  TCanvas *cxdist = new TCanvas("cmdist", "Mom Dist", 1200, 1600);
  cxdist->Divide(3, 3);

  const int nhist = 12;
  int i = 0;
  for (const auto &it : cutiso) {
    cxdist->cd((i % nhist) + 1);
    mgs[it]->Draw("AP");
    i++;
    if (i % nhist == 0) {
      cxdist->Print(Form("analysis/momdist_%s.pdf%s", iso.c_str(), i == nhist ? "(" : ""));
      cxdist->Clear("D");
    }
  }
  cxdist->Print(Form("analysis/momdist_%s.pdf%s", iso.c_str(), i < nhist ? "" : ")"));
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Root file saving
  TFile *file = new TFile(Form("analysis/momdist_%s.root", iso.c_str()), "RECREATE");
  for (const auto &it : mgs) {
    it.second->Write();
  }
  file->Close();
  delete file;
  /////////////////////////////////////////////////////////////////////
}

std::vector<std::string> KOBRA::GetCuts() {
  std::vector<std::string> cuts;
  for (const auto &it : cutgs) {
    cuts.push_back(it.first);
  }
  return cuts;
}

void KOBRA::LoadMomDistAnalysis(std::string iso) {
  TFile *file = new TFile(Form("analysis/momdist_%s.root", iso.c_str()), "READ");
  std::map<std::string, TMultiGraph *> mgs;

  TList *list = file->GetListOfKeys();
  for (int i = 0; i < list->GetEntries(); i++) {
    std::string key = list->At(i)->GetName();
    mgs[key] = static_cast<TMultiGraph *>(file->Get(key.c_str()));
  }

  std::string selection = "o20";
  TCanvas *cxdist = new TCanvas("cmdist", "Mom Dist", 800, 800);
  auto mg = mgs[selection];

  TList *list2 = mg->GetListOfGraphs();
  for (int i = 0; i < list2->GetEntries(); i++) {
    auto gr = static_cast<TGraph *>(list2->At(i));
    std::string name = gr->GetName();
    if (name[name.size() - 1] == '0') {
      gr->Draw("AP");
    }
  }

  file->Close();
  delete file;
}

TH1 *KOBRA::F3PlaAccHis(const char *hname, const char *cut, bool flagDraw) {
  Int_t plax[2] = {-50, 50};
  Int_t play[2] = {-50, 50};

  tree->Draw(Form("f3play:f3plax>>%s(200,-100,100,200,-100,100)", hname), cut, "goff");
  TH1 *h = static_cast<TH1 *>(gDirectory->Get(hname));
  if (flagDraw) {
    h->Draw();
    gPad->SetLogz();
    // Draw the acceptance region of the plastic
    TBox *box = new TBox(plax[0], play[0], plax[1], play[1]);
    box->SetLineColor(kRed);  // 빨간색 테두리
    box->SetLineWidth(2);     // 테두리 두께
    box->SetFillStyle(0);     // 내부 채우기 없음
    box->Draw();
  }
  // 사각형 내부의 이벤트 수를 계산합니다.
  int nInside = 0;
  if (h) {
    // x, y축의 bin 범위 계산
    int xBinMin = h->GetXaxis()->FindBin(plax[0]);
    int xBinMax = h->GetXaxis()->FindBin(plax[1]);
    int yBinMin = h->GetYaxis()->FindBin(play[0]);
    int yBinMax = h->GetYaxis()->FindBin(play[1]);

    for (int x = xBinMin; x <= xBinMax; ++x) {
      for (int y = yBinMin; y <= yBinMax; ++y) {
        nInside += h->GetBinContent(x, y);
      }
    }
  }
  printf("Plastic acceptance: %f\n", static_cast<double>(nInside) / h->Integral());
  return h;
}

TH1 *KOBRA::F3SSDAccHis(const char *hname, const char *cut, bool flagDraw) {
  Int_t ssdx[2] = {-25, 25};
  Int_t ssdy[2] = {-25, 25};

  tree->Draw(Form("f3ssdy:f3ssdx>>%s(200,-100,100,200,-100,100)", hname), cut, "goff");
  TH1 *h = static_cast<TH1 *>(gDirectory->Get(hname));
  if (flagDraw) {
    h->Draw();
    gPad->SetLogz();
    // Draw the acceptance region of the SSD
    TBox *box = new TBox(ssdx[0], ssdy[0], ssdx[1], ssdy[1]);
    box->SetLineColor(kRed);  // 빨간색 테두리
    box->SetLineWidth(2);     // 테두리 두께
    box->SetFillStyle(0);     // 내부 채우기 없음
    box->Draw();
  }
  // 사각형 내부의 이벤트 수를 계산합니다.
  int nInside = 0;
  if (h) {
    // x, y축의 bin 범위 계산
    int xBinMin = h->GetXaxis()->FindBin(ssdx[0]);
    int xBinMax = h->GetXaxis()->FindBin(ssdx[1]);
    int yBinMin = h->GetYaxis()->FindBin(ssdy[0]);
    int yBinMax = h->GetYaxis()->FindBin(ssdy[1]);

    for (int x = xBinMin; x <= xBinMax; ++x) {
      for (int y = yBinMin; y <= yBinMax; ++y) {
        nInside += h->GetBinContent(x, y);
      }
    }
  }
  printf("SSD acceptance: %f\n", static_cast<double>(nInside) / h->Integral());
  return h;
}

void KOBRA::DrawLength(const char *bname, const char *cut, bool flag01, int nbins) {
  // Remove all spaces from bname and split by ','
  std::string bname_str = bname ? bname : "";
  bname_str.erase(remove_if(bname_str.begin(), bname_str.end(), ::isspace), bname_str.end());

  std::vector<std::string> branches;
  size_t start = 0;
  while (true) {
    size_t pos = bname_str.find(',', start);
    if (pos == std::string::npos) {
      branches.push_back(bname_str.substr(start));
      break;
    }
    branches.push_back(bname_str.substr(start, pos - start));
    start = pos + 1;
  }

  TString draw_formula = "";
  TString hist_name = "h";
  if (branches.size() == 1) {
    auto br = tree->GetBranch(branches[0].c_str());
    if (br && strcmp(br->GetClassName(), "TClonesArray") == 0)
      draw_formula = Form("%s@.GetEntriesFast()", branches[0].c_str());
    else
      draw_formula = Form("Length$(%s)", branches[0].c_str());
    hist_name += TString::Format("%s_length", branches[0].c_str());
  } else {
    // Multiply together lengths for each branch
    std::vector<TString> formulas;
    for (const auto &b : branches) {
      auto br = tree->GetBranch(b.c_str());
      if (br && strcmp(br->GetClassName(), "TClonesArray") == 0)
        formulas.push_back(Form("%s@.GetEntriesFast()", b.c_str()));
      else
        formulas.push_back(Form("Length$(%s)", b.c_str()));
      hist_name += b.c_str();
      hist_name += "x";
    }
    // Remove last 'x'
    if (hist_name.EndsWith("x")) hist_name.Remove(hist_name.Length() - 1, 1);

    // Join with '*'
    draw_formula = formulas[0];
    for (size_t i = 1; i < formulas.size(); ++i) {
      draw_formula += "*";
      draw_formula += formulas[i];
    }
    hist_name += "_length";
  }
  if (flag01) {
    tree->Draw(Form("%s > 0 >>%s(2,-0.5,1.5)", draw_formula.Data(), hist_name.Data()), cut, "goff");
  } else {
    tree->Draw(Form("%s>>%s(%d,-0.5,%d.5)", draw_formula.Data(), hist_name.Data(), nbins, nbins - 1), cut, "goff");
  }
  TH1 *h = static_cast<TH1 *>(gDirectory->Get(hist_name));
  h->SetTitle(Form("Length of %s;Length;Count", bname_str.c_str()));
  h->SetMarkerSize(2.0);  // 폰트 사이즈 키우기 (숫자 크기 증가)
  h->Draw("hist,text0");
}
