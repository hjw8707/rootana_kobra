#include "kobra.hxx"

#include "curry.hxx"

#include "TFile.h"
#include "TSystem.h"
#include "TCutG.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TSystemDirectory.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TChainElement.h"
#include "TSelectorEntries.h"
#include "TColor.h"
#include "TGaxis.h"
#include "TMath.h"
#include "TMultiGraph.h"
#include "THeader.hxx"
#include "TScalerData.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

ClassImp(KOBRA);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for cross section runs
std::vector<int> KOBRA::o18 = {251, 252, 253, 254};
std::vector<int> KOBRA::o19 = {259, 260, 261, 262, 263, 264, 265, 266, 267,
                               271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290,
                               291, 292, 293, 294, 295, 296, 297, 298, 299};

std::vector<int> KOBRA::o20 = {304, 305, 306, 307, 308, 309, 310};

std::vector<int> KOBRA::o21 = {316,
                               318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345,
                               347};

std::vector<int> KOBRA::o22 = {223, 224, 225, 226, // Right after the newly adjusted primary beam
                               348, 349, 350, 351, 352,
                               361, 362,
                               364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376,
                               378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400,
                               402, 403, 404};

std::vector<int> KOBRA::ne24 = {410, 411, 412, 413, 415};

std::vector<int> KOBRA::ne25 = {420, 421, 422};

std::vector<int> KOBRA::ne26 = {425, 426, 427, 428, 429, 430, 431,
                                433, 434, 435, 436, 437, 438, 439, 440};

std::vector<std::string> KOBRA::o18_iso = {
    "c13", "c14", "c15",
    "n15", "n16", "n17",
    "o17", "o18", "o19",
    "f19", "f20", "f21",
    "ne21", "ne22"};
std::vector<std::string> KOBRA::o19_iso = { // ApplyOffsetToCut(-0.005)
    "c13", "c14", "c15", "c16",
    "n15", "n16", "n17", "n18",
    "o17", "o18", "o19", "o20",
    "f19", "f20", "f21", "f22",
    "ne21", "ne22", "ne23", "ne24",
    "na25"};

std::vector<std::string> KOBRA::o20_iso = { // ApplyOffsetToCut(-0.014,0)
    "c13", "c14", "c15", "c16",
    "n15", "n16", "n17", "n18", "n19",
    "o18", "o19", "o20", "o21",
    "f20", "f21", "f22", "f23",
    "ne22", "ne23", "ne24",
    "na25", "na26"};

std::vector<std::string> KOBRA::o21_iso = { // ApplyOffsetToCut(-0.02,-0.1)
    "c14", "c15", "c16", "c17",
    "n16", "n17", "n18", "n19",
    "o18", "o19", "o20", "o21", "o22",
    "f21", "f22", "f23", "f24",
    "ne23", "ne24", "ne25",
    "na25", "na26", "na27",
    "mg28"};

std::vector<std::string> KOBRA::o22_iso = { // ApplyOffsetToCut(-0.03)
    "c15", "c16", "c17",
    "n17", "n18", "n19",
    "o19", "o20", "o21", "o22",
    "f22", "f23", "f24",
    "ne24", "ne25", "ne26",
    "na27", "na28",
    "mg29"};

std::vector<std::string> KOBRA::ne24_iso = { // ApplyOffsetToCut(-0.005)
    "ne24", "na25", "na26", "mg28", "al29", "al30"};

std::vector<std::string> KOBRA::ne25_iso = { // ApplyOffsetToCut(-0.005, -0.1)
    "ne24", "na25", "na26", "na27", "mg28", "mg29", "al30", "al31"};

std::vector<std::string> KOBRA::ne26_iso = { // ApplyOffsetToCut(-0.005, -0.1)
    "f22", "f23", "ne24", "na25", "na26", "na27", "mg28", "mg29", "al30", "al31"};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for mom distribution runs
std::vector<int> KOBRA::mom_14 = {656, 657};                         // ApplyOffsetToCut(0.04)
std::vector<int> KOBRA::mom_12 = {643, 644, 645, 646, 647, 648};     // ApplyOffsetToCut(0.04)
std::vector<int> KOBRA::mom_10 = {639, 640, 641, 642};               // ApplyOffsetToCut(0.035)
std::vector<int> KOBRA::mom_08 = {620, 621, 622, 623, 625};          // ApplyOffsetToCut(0.025)
std::vector<int> KOBRA::mom_07 = {651, 652, 653, 655};               // ApplyOffsetToCut(0.025)
std::vector<int> KOBRA::mom_06 = {615, 616, 617};                    // ApplyOffsetToCut(0.02, -0.1)
std::vector<int> KOBRA::mom_05 = {660, 661};                         // ApplyOffsetToCut(0.015, -0.1)
std::vector<int> KOBRA::mom_04 = {610, 611, 612};                    // ApplyOffsetToCut(0.015, -0.1)
std::vector<int> KOBRA::mom00 = {601, 602, 603, 604};                // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mom04 = {605, 606, 607};                     // ApplyOffsetToCut(-0.005,0.1)
std::vector<int> KOBRA::mom07 = {590, 592, 593, 594, 595, 596, 598}; // ApplyOffsetToCut(-0.01,0.2)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static variables for mom distribution runs
std::vector<int> KOBRA::mon_10 = {775, 776, 777, 778, 779 };    // ApplyOffsetToCut(0.05)
std::vector<int> KOBRA::mon_08 = {770, 771 }; // ppac no
std::vector<int> KOBRA::monn_08 = {763, 764, 765 };
std::vector<int> KOBRA::mon_07 = {780, 781 };  // ApplyOffsetToCut(0.032)
std::vector<int> KOBRA::mon_06 = {754, 755, 756, 757, 758, 759 };
std::vector<int> KOBRA::mon_05 = {782, 783, 784, 785 };
std::vector<int> KOBRA::mon_04 = {788, 789, 750, 751, 752};                              // ApplyOffsetToCut(?)
std::vector<int> KOBRA::monn_04 = {750, 751, 752};                              // ApplyOffsetToCut(?)
std::vector<int> KOBRA::mon_03 = {786, 787};                              // ApplyOffsetToCut(0.005)
std::vector<int> KOBRA::mon_02 = {746, 748};                              // ApplyOffsetToCut(0.005)
std::vector<int> KOBRA::mon_01 = {738, 739, 740};                 // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mon00 = {706, 707, 708};                 // ApplyOffsetToCut(0, 0)
std::vector<int> KOBRA::mon01 = {713, 714, 715, 716, 717, 718, 719};  // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon02 = {720, 721, 722, 723, 724, 725};            // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon04 = {726, 727, 728, 729, 730 };            // ApplyOffsetToCut(-0.005)
std::vector<int> KOBRA::mon06 = {731, 732, 733 };            // ApplyOffsetToCut(-0.01)
std::vector<int> KOBRA::mon10 = {734, 735, 736, 737};            // ApplyOffsetToCut(-0.02)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

KOBRA::KOBRA()
{
  Initilize();
}

KOBRA::KOBRA(const char *file)
{
  Initilize();
  LoadTree(file);
}
KOBRA::KOBRA(std::string file)
{
  Initilize();
  LoadTree(file);
}
KOBRA::KOBRA(std::vector<std::string> files)
{
  Initilize();
  LoadTree(files);
}
KOBRA::KOBRA(int run)
{
  Initilize();
  LoadTree(run);
  runNs.push_back(run);
  RunSetting(run);
}
KOBRA::KOBRA(std::vector<int> runs)
{
  Initilize();
  LoadTree(runs);
  for (const auto &it : runs)
    runNs.push_back(it);
  RunSetting(runs[0]);
}
KOBRA::KOBRA(int runb, int rune)
{
  Initilize();
  LoadTree(runb, rune);
  for (int i = runb; i <= rune; i++)
    runNs.push_back(i);
  RunSetting(runb);
}

KOBRA::~KOBRA()
{
  if (tree)
    delete tree;
}

void KOBRA::Initilize()
{
  tree = new TChain("kobra", "kobra");
  centBrho = 1.5065;
  tofOff = 0;
  useF1 = false;
  useF2orF3 = false;
  useSSDorPla = true;
  // gcut = "f3ssd@.GetEntriesFast() > 0 && f3ssd@.GetEntriesFast() < 3";
  gcut = "";
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

Bool_t KOBRA::LoadTree(const char *filename)
{
  // Check if file exists
  if (gSystem->AccessPathName(filename))
  {
    std::cout << "File " << filename << " does not exist." << std::endl;
    return false;
  }

  // Open the ROOT file
  TFile file(filename, "READ");

  // Check if the file is valid
  if (!file.IsOpen() || file.IsZombie())
  {
    std::cout << "File " << filename << " is not a valid ROOT file." << std::endl;
    return false;
  }

  // Check if the TTree exists in the file
  TTree *tr = dynamic_cast<TTree *>(file.Get("kobra"));
  if (!tr)
  {
    std::cout << "TTree kobra does not exist in file " << filename << "." << std::endl;
    return false;
  }

  THeader *head = dynamic_cast<THeader *>(file.Get("header"));
  if (!head)
  {
    std::cout << "THeader does not exist in file " << filename << "." << std::endl;
    return false;
  }

  // Add the file to the TChain
  tree->Add(filename);
  headers.push_back(head);
  std::cout << "TTree kobra from file " << filename << " added to TChain." << std::endl;
  return true;
}

Bool_t KOBRA::LoadTree(std::string file)
{
  return LoadTree(file.c_str());
}

Bool_t KOBRA::LoadTree(std::vector<std::string> files)
{
  Bool_t result = true;
  for (const auto &it : files)
  {
    result = result && LoadTree(it.c_str());
  }
  return result;
}

Bool_t KOBRA::LoadTree(int run)
{
  return LoadTree(Form("root/output%08d.root", run));
}

Bool_t KOBRA::LoadTree(std::vector<int> runs)
{
  Bool_t result = true;
  for (const auto &it : runs)
  {
    result = result && LoadTree(it);
  }
  return result;
}

Bool_t KOBRA::LoadTree(int runb, int rune)
{
  if (rune < runb)
    return false;

  Bool_t result = true;
  for (int run = runb; run <= rune; run++)
  {
    result = result && LoadTree(run);
  }
  return result;
}

std::vector<std::string> KOBRA::split(const std::string &s, char delimiter, bool flagEmpty)
{
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
  {
    if (flagEmpty || !token.empty())
      tokens.push_back(token);
  }
  return tokens;
}

Bool_t KOBRA::LoadDB(const char *filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
    return false;
  }

  std::string line;
  while (std::getline(file, line))
  {
    // #으로 시작하는 주석을 제거
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos)
      line = line.substr(0, commentPos);

    // 공백만 있는 줄은 무시
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
      continue;

    std::vector<std::string> tokens = split(line, '\t');
    Int_t run;
    try
    {
      run = std::stoi(tokens[0]);
    }
    catch (...)
    {
      continue;
    }

    Double_t brho;
    try
    {
      brho = std::stof(tokens[7]);
    }
    catch (...)
    {
      continue;
    }
    brhoMap[run] = brho;

    Double_t f1slit;
    try
    {
      f1slit = std::stof(tokens[11]) + std::stof(tokens[12]);
    }
    catch (...)
    {
      continue;
    }
    f1slitMap[run] = f1slit;
  }

  file.close();
  return true;
}

void KOBRA::RunSetting(int run)
{
  try
  {
    SetBrho(brhoMap[run]);
  }
  catch (...)
  {
  }
  // SetBrho(1.356);
  SetUseF1(true);
  // AddCuts("./cut");
  if (run >= 588)
    SetUsePla();
  else
    SetUseSSD();

  ca = NULL;
  tree->SetBranchAddress("scaler", &ca);
}

void KOBRA::SetAlias()
{
  if (!tree)
    return;

  std::cout << "Set Alias" << std::endl;
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
  tree->SetAlias("f3x", "760.*tan(f3a/1000.)+f3dppac.x");
  tree->SetAlias("f3y", "760.*tan(f3b/1000.)+f3dppac.y");

  tree->SetAlias("db", "f1uppacx.x");
  tree->SetAlias("tof", Form("(f3uppt.tsum - f2dppt.tsum) + 103.8 + %f", tofOff));
  tree->SetAlias("tofp", Form("(f3pla.GetTAve() - f2dppt.tsum) + 103.8 - 6.62 + %f", tofOff));
  //  tree->SetAlias("de", "Max$(f2ssd.acal)");
  tree->SetAlias("ycor", "(-0.00245304*f3uppac.y^2-0.034376*f3uppac.y+19.8065)/20");

  if (useSSDorPla)
  {
    if (useF2orF3)
      tree->SetAlias("de", "Max$(f2ssd.acal)");
    else
      tree->SetAlias("de", "Max$(f3ssd.acal)/ycor");
  }
  else
  {
    tree->SetAlias("de", "sqrt(f3dpla.al*f3dpla.ar)*0.0075");
    // tree->SetAlias("de", "sqrt(f3dpla.al*f3dpla.ar)");
  }
  tree->SetAlias("brho", Form("(1+db/4100)*%f", centBrho));

  tree->SetAlias("beta", "12398.5/tof/299."); // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gamma", "1/sqrt(1-beta*beta)");
  tree->SetAlias("pp", "gamma*beta");

  tree->SetAlias("betap", "12398.5/tofp/299."); // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gammap", "1/sqrt(1-betap*betap)");
  tree->SetAlias("ppp", "gammap*betap");

  if (useF1)
  {
    tree->SetAlias("aoq", "brho/(3.1*pp)");
    tree->SetAlias("aoqp", "brho/(3.1*ppp)");
  }
  else
    tree->SetAlias("aoq", Form("%f/(3.1*pp)",centBrho));
  tree->SetAlias("AoQ", "0.941841*aoq+0.095597");
  tree->SetAlias("AoQp", "0.941841*aoqp+0.095597");
  tree->SetAlias("corfac", "log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
  tree->SetAlias("z", "sqrt(de)*beta/sqrt(corfac)");

  //  if (useSSDorPla) {
  tree->SetAlias("Z", "21.540118*z+0.444610");
  //    else {
  //      tree->SetAlias("Z","4.8037*z-1.8");}

  ////////////////////////////////////////////
  // de-tof cut
  tree->SetAlias("detof", "de-0.12*(tof-230)");
  tree->SetAlias("detof_f", "abs(detof - 25) < 2.1");
  tree->SetAlias("detof_o", "abs(detof - 20) < 2.5");
  tree->SetAlias("detof_n", "abs(detof - 15) < 2.");
  tree->SetAlias("detof_c", "abs(detof - 11) < 2.");
  ////////////////////////////////////////////
}

void KOBRA::AddCut(const char *filename)
{

  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
    return;
  }

  int linen = 0;
  std::string line;
  TCutG *cut;
  while (std::getline(file, line))
  {
    // #으로 시작하는 주석을 제거
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos)
      line = line.substr(0, commentPos);

    // 공백만 있는 줄은 무시
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
      continue;

    std::vector<std::string> tokens = split(line, ' ', false);
    if (linen == 0)
    {
      cut = new TCutG(tokens[0].c_str());
      if (tokens.size() > 1)
        cut->SetTitle(tokens[1].c_str());
    }
    else
    {
      try
      {
        cut->AddPoint(std::stof(tokens[0]), std::stof(tokens[1]));
      }
      catch (...)
      {
        continue;
      }
    }
    linen++;
  }
  if (cut)
  {
    cut->SetVarX("AoQ");
    cut->SetVarY("Z");
    cut->SetFillStyle(1000);
    cut->SetLineStyle(2);
    cutgs[cut->GetName()] = cut;
  }
  file.close();
}

void KOBRA::AddCuts(const char *path)
{
  for (const auto it : cutgs)
  {
    delete (it.second);
  }
  cutgs.clear();
  TSystemDirectory dir(path, path);

  TList *filesList = dir.GetListOfFiles();
  if (!filesList)
  {
    std::cerr << "Error: " << path << " is not a valid directory or cannot be accessed.\n";
    return;
  }

  TIter next(filesList);
  TSystemFile *file;
  while ((file = (TSystemFile *)next()))
  {
    TString fileName = file->GetName();
    if (!file->IsDirectory() && fileName.EndsWith("txt"))
    {
      AddCut(Form("%s/%s", path, fileName.Data()));
    }
  }

  SetCutStyle(2, 2);
}

void KOBRA::DrawPID0(const char *cut)
{
  if (!tree)
    return;

  std::string cuts = gcut;
  if (cut)
    if (cuts.empty())
      cuts += Form("%s", cut);
    else
      cuts += Form("&& %s", cut);
  tree->Draw("de:tof>>hpid0(400,150,350,400,0,150)", cuts.c_str(), "col");
}

void KOBRA::DrawPID(const char *cut)
{
  if (!tree)
    return;

  std::string cuts = gcut;
  if (cut)
    if (cuts.empty())
      cuts += Form("%s", cut);
    else
      cuts += Form("&& %s", cut);
  tree->Draw("z:aoq>>hpid(400,1.5,3,400,0,1)", cuts.c_str(), "col");
}

void KOBRA::DrawPIDC(Int_t show, const char *cut)
{
  if (show < 0 || show > 2)
    return;
  if (!tree)
    return;

  std::string cuts = gcut;
  if (cut)
    if (cuts.empty())
      cuts += Form("%s", cut);
    else
      cuts += Form("&& %s", cut);
  auto c1 = new TCanvas(Form("cpid_r%d", runNs.front()), "PID", 1600, 1200);
  tree->Draw(Form("Z:AoQ>>hpidc_r%d(500,2.15,2.75,500,4,15)", runNs.front()), cuts.c_str(), "col");

  TH2 *h2 = static_cast<TH2 *>(gDirectory->Get(Form("hpidc_r%d", runNs.front())));

  std::string runstr;
  if (runNs.size() > 1)
    runstr = Form("Run %d-%d", runNs.front(), runNs.back());
  else
    runstr = Form("Run %d", runNs.front());

  const char *showcase[3] = {"", ": Count", ": Rate [pps]"};
  h2->SetTitle(Form("PID for %s%s;A/Q;Z", runstr.c_str(), showcase[show]));

  if (show > 0)
  {
    TLatex text;
    text.SetTextFont(62);
    text.SetTextSize(0.03);
    text.SetTextColor();
    Double_t time;
    time = GetElapsedTime();
    for (const auto &it : cutgs)
    {
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
      text.SetTextColor(kOrange+7);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      text.DrawLatex(xmax, ymax, Form("^{%s}%s", aaa.Data(), zzz.Data()));

      text.SetTextFont(52);
      text.SetTextColor(kMagenta +1);
      text.SetTextSize(0.03);
      text.SetTextAlign(22);
      TString a;

      if (show == 1)
        a = Form("%d", count);
      else
        a = Form("%.3f", float(count / time));

      text.DrawLatex(x, y, a.Data());
    }
    std::string timestr;
    Int_t itime = time;
    text.SetTextAlign(12);
    if (itime > 3600)
      timestr = Form("%02d:%02d:%02d", itime / 3600, (itime % 3600) / 60, itime % 60);
    else if (itime > 60)
      timestr = Form("%02d:%02d", itime / 60, itime % 60);
    else
      timestr = "";

    text.DrawLatexNDC(0.15, 0.92, Form("Brho: %.4f Tm / Time: %.0f sec. or %s", GetBrho(), time, timestr.c_str()));
  }
}

void KOBRA::SetZ(Int_t iz1, Double_t z1, Int_t iz2, Double_t z2)
{
  Double_t a = static_cast<Double_t>(iz2 - iz1) / (z2 - z1);
  Double_t b = static_cast<Double_t>(iz1) - a * z1;
  SetZ(a, b);
}

void KOBRA::SetAoQ(Double_t iz1, Double_t z1, Double_t iz2, Double_t z2)
{
  Double_t a = static_cast<Double_t>(iz2 - iz1) / (z2 - z1);
  Double_t b = static_cast<Double_t>(iz1) - a * z1;
  SetAoQ(a, b);
}

void KOBRA::PrintSetting(std::ostream &out)
{
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

void KOBRA::CountIsotopes()
{
  ////////////////////////////////////////////////////////////
  // Counting
  std::cout << "=========================================" << std::endl;
  std::cout << "   Counts of Isotopes " << std::endl;
  std::cout << "=========================================" << std::endl;
  for (const auto &it : cutgs)
  {
    std::cout << " ";
    std::cout << it.second->GetTitle();
    std::cout << ": ";
    std::cout << GetEntries(it.second->GetName());
    std::cout << std::endl;
  }
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////
}

void KOBRA::RateIsotopes()
{
  Double_t time = GetElapsedTime();

  ////////////////////////////////////////////////////////////
  // Counting
  std::cout << "=========================================" << std::endl;
  std::cout << "   Rates of Isotopes " << std::endl;
  std::cout << "=========================================" << std::endl;
  for (const auto &it : cutgs)
  {
    std::cout << " ";
    std::cout << it.second->GetTitle();
    std::cout << ": ";
    std::cout << static_cast<double>(GetEntries(it.second->GetName()) / time);
    std::cout << std::endl;
  }
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////
}

void KOBRA::DrawAllCut()
{

  for (const auto &it : cutgs)
  {
    it.second->Draw("SAME");
  }
}

std::vector<TH1 *> KOBRA::DrawXDist(const char *cut, bool flagDraw)
{
  TString cutname;
  if (cut)
    cutname = cut;
  else
    cutname = "nocut";

  TString drawopt;
  TCanvas *c1;
  if (flagDraw)
  {
    c1 = new TCanvas(Form("cxd_r%d_%s", runNs.front(), cutname.Data()),
                     "X Distribution", 1600, 400);
    c1->Divide(4, 1);
  }
  else
    drawopt = "goff";

  std::vector<TH1 *> hists;

  if (flagDraw)
    c1->cd(1);
  tree->Draw(Form("f1x>>hf1x_r%d_%s(100,-200,200)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf1x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw)
    c1->cd(2);
  tree->Draw(Form("f2x>>hf2x_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf2x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw)
    c1->cd(3);
  tree->Draw(Form("f3x>>hf3x_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf3x_r%d_%s", runNs.front(), cutname.Data()))));

  if (flagDraw)
    c1->cd(4);
  tree->Draw(Form("f3uppac.x>>hf3ux_r%d_%s(120,-60,60)", runNs.front(), cutname.Data()), cut, drawopt.Data());
  hists.push_back(static_cast<TH1 *>(gDirectory->Get(Form("hf3ux_r%d_%s", runNs.front(), cutname.Data()))));

  return hists;
}

TH1 *KOBRA::DrawMomDist(const char *cut, Bool_t flagRate, Bool_t flagDraw, Double_t binSize, Bool_t flagOff)
{
  TLatex text;
  text.SetTextFont(62);
  text.SetTextSize(0.03);
  text.SetTextColor();

  TString cutname;
  if (cut)
    cutname = cut;
  else
    cutname = "nocut";

  gStyle->SetOptStat(0);

  Int_t nBin = TMath::Nint(4 / binSize) * 2;
  if (flagOff)
    nBin--;

  Double_t lim = nBin / 2 * binSize * 40;
  if (flagOff)
    lim -= 20;

  TString drawOpt;
  if (flagDraw)
  {
    auto c1 = new TCanvas(Form("cmd_r%d_%s", runNs.front(), cutname.Data()),
                          "Mom. Distribution", 1200, 800);
    drawOpt = "text0";
  }
  else
    drawOpt = "goff";

  tree->Draw(Form("f1x>>hmd_r%d_%s(%d,%f,%f)", runNs.front(), cutname.Data(),
                  nBin, -lim, lim),
             cut,
             drawOpt.Data());

  gStyle->SetPaintTextFormat();
  auto h1 = static_cast<TH1 *>(gDirectory->Get(Form("hmd_r%d_%s", runNs.front(), cutname.Data())));
  Double_t time = GetElapsedTime();
  if (flagRate)
  {
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

  std::string timestr;
  Int_t itime = time;
  if (itime > 3600)
    timestr = Form("%02d:%02d:%02d", itime / 3600, (itime % 3600) / 60, itime % 60);
  else if (itime > 60)
    timestr = Form("%02d:%02d", itime / 60, itime % 60);
  else
    timestr = "";

  if (flagDraw)
    text.DrawLatexNDC(0.15, 0.92, Form("Brho: %.4f Tm / Time: %.0f sec. or %s", GetBrho(), time, timestr.c_str()));
  return h1;
}

TGraphErrors *KOBRA::GetMomDistGraph(Double_t center, const char *cut, Double_t binSize, Double_t leftLimit, Double_t rightLimit, Bool_t flagOff, Double_t scale, Bool_t flagUseF1)
{
  auto h1 = DrawMomDist(cut, true, false, binSize, flagOff);

  TGraphErrors *gr = new TGraphErrors;
  gr->SetTitle(Form("Momentum Distribution of %s;Momentum Dispersion (%%);Rate per Mom. %% (pps/%%)", cut));

  if (flagUseF1) {
    for (Int_t i = 1; i <= h1->GetNbinsX(); i++)
      {
	Double_t curPoint = h1->GetBinCenter(i) / 40.;
	if (curPoint < leftLimit || curPoint > rightLimit)
	  continue;
	//	std::cout << cut << ", " << h1->GetBinContent(i) << " " << scale << " " << binSize <<std::endl;
	gr->AddPoint(curPoint + center, h1->GetBinContent(i) * scale / binSize);
	gr->SetPointError(gr->GetN() - 1, binSize * 0.5, h1->GetBinError(i) * scale / binSize);
      }}
  else {
    Double_t time = GetElapsedTime();
    Int_t event = tree->GetEntries(cut);
    //    std::cout << cut << ": " << time << " " << event << std::endl;
    gr->AddPoint(center, event/time * scale / binSize);
    gr->SetPointError(0, 0.05, TMath::Sqrt(event)/time * scale / binSize); 
  }
  gr->SetMarkerStyle(20);
  gr->SetMarkerColor(1);
  gr->SetMarkerSize(2);
  return gr;
}

std::vector<TGraphErrors *> KOBRA::GetMomDistGraphs(Double_t center, Double_t binSize, Double_t leftLimit, Double_t rightLimit, Bool_t flagOff, Int_t bias)
{
  Double_t scale;
  scale = 1. / (GetPPACEff(2) * GetPPACEff(3) * GetLiveTime());

  std::vector<TGraphErrors *> momgraphs;
  for (const auto it : cutgs)
  {
    std::string cutname = it.first;
    cutname.erase(std::remove_if(cutname.begin(), cutname.end(), [](char c)
                                 { return std::isdigit(c); }),
                  cutname.end());
    //    std::cout << cutname << " " << GetPPACEff(1, NULL, true, cutname, bias) << std::endl;
    // scale *= (1 / GetPPACEff(1, NULL, true, cutname, bias));
    //    std::cout << scale << std::endl;
    TGraphErrors *gr = GetMomDistGraph(center, it.first.c_str(), binSize,
                                       leftLimit, rightLimit, flagOff,
                                       scale / GetPPACEff(1, NULL, true, cutname, bias));
    momgraphs.push_back(gr);
  }
  return momgraphs;
}


std::vector<TGraphErrors *> KOBRA::GetMomDistGraphsNe(std::vector<std::string> iso, Double_t center, Double_t binSize, Double_t leftLimit, Double_t rightLimit, Bool_t flagOff, Int_t bias, Bool_t flagUseF1)
{
  Double_t scale;
  scale = 1. / (GetPPACEff(2) * GetPPACEff(3) * GetLiveTime());

  std::vector<TGraphErrors *> momgraphs;
  for (const auto it : iso)
    {
      std::string cutname = it;
      cutname.erase(std::remove_if(cutname.begin(), cutname.end(), [](char c)
								   { return std::isdigit(c); }),
	cutname.end());
      // scale *= (1 / GetPPACEff(1, NULL, true, cutname, bias));
      //      std::cout << GetPPACEff(1, NULL, true, cutname, bias) << std::endl;
      TGraphErrors *gr;
      if (flagUseF1) 
	gr = GetMomDistGraph(center, it.c_str(), binSize,
			     leftLimit, rightLimit, flagOff,
			     scale / GetPPACEff(1, NULL, true, cutname, bias),
			     flagUseF1);
      else
	gr = GetMomDistGraph(center, it.c_str(), binSize,
			     leftLimit, rightLimit, flagOff,
			     scale, flagUseF1);
      momgraphs.push_back(gr);
    }
  return momgraphs;
}


Double_t KOBRA::GetElapsedTime()
{

  std::vector<Long64_t> nEntries;

  TObjArray *fileElements = tree->GetListOfFiles();
  for (TObject *op : *fileElements)
  {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile f{chainElement->GetTitle()};
    TTree *tree = f.Get<TTree>(chainElement->GetName());
    nEntries.push_back(tree->GetEntries());
  }

  std::vector<Long64_t> nSum;
  for (auto it = nEntries.begin(); it != nEntries.end(); it++)
  {
    Int_t sum = std::accumulate(nEntries.begin(), it + 1, 0);
    nSum.push_back(sum);
  }

  Double_t totalTime = 0;
  for (auto it = nSum.begin(); it != nSum.end(); it++)
  {
    Int_t n0, n1;
    if (it == nSum.begin())
    {
      for (int i = 0; i <= tree->GetEntries(); i++)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n0 = static_cast<TScalerData *>(ca->At(0))->reft;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n1 = static_cast<TScalerData *>(ca->At(0))->reft;
          break;
        }
      }
    }
    else
    {
      for (int i = *(it - 1); i <= tree->GetEntries(); i++)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n0 = static_cast<TScalerData *>(ca->At(0))->reft;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n1 = static_cast<TScalerData *>(ca->At(0))->reft;
          break;
        }
      }
    }
    totalTime += (Double_t)(n1 - n0);
  }
  return totalTime;
}

Long64_t KOBRA::GetEntries(const char *selection)
{
  TSelectorEntries *s = new TSelectorEntries(selection);
  tree->Process(s);
  tree->SetNotify(nullptr);
  Long64_t res = s->GetSelectedRows();
  delete s;
  return res;
}

void KOBRA::DrawPPACEff(const char *cut)
{
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

TGraph *KOBRA::PPACRate()
{

  TGraph *gr = new TGraph;

  Double_t lastts = -1;
  Double_t lastppac = -1;
  for (Int_t i = 0; i < tree->GetEntries(); i += 10)
  {
    tree->GetEntry(i);
    if (!ca || ca->GetEntriesFast() == 0)
      continue;
    time_t start_ts = GetHeader(tree->GetTreeNumber())->GetStartTimeStamp();
    time_t day0 = start_ts - (start_ts % 86400) - 9 * 3600; // 00:00:00 of the day
    if (i > 1)
    {
      Double_t dt = (double)(static_cast<TScalerData *>(ca->At(0))->ts - lastts);
      Double_t dppac = (double)(static_cast<TScalerData *>(ca->At(0))->ppac - lastppac);
      if (dt != 0)
      {
        gr->AddPoint(day0 + static_cast<TScalerData *>(ca->At(0))->reft, dppac / (dt * 1.E-8));
      }
    }
    lastts = (double)(static_cast<TScalerData *>(ca->At(0))->ts);
    lastppac = (double)(static_cast<TScalerData *>(ca->At(0))->ppac);
  }

  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  gr->GetXaxis()->SetNdivisions(505);

  gr->SetTitle("PPAC Or Rate;Time [s];Rate [pps]");
  gr->Draw("AL");
  return gr;
}

TGraph *KOBRA::TriggerRate()
{

  TGraph *gr = new TGraph;

  Double_t lastts = -1;
  Double_t lasttrig = -1;
  for (Int_t i = 0; i < tree->GetEntries(); i += 10)
  {
    tree->GetEntry(i);
    if (!ca || ca->GetEntriesFast() == 0)
      continue;
    time_t start_ts = GetHeader(tree->GetTreeNumber())->GetStartTimeStamp();
    time_t day0 = start_ts - (start_ts % 86400) - 9 * 3600; // 00:00:00 of the day
    if (i > 1)
    {
      Double_t dt = (double)(static_cast<TScalerData *>(ca->At(0))->ts - lastts);
      Double_t dtrig = (double)(static_cast<TScalerData *>(ca->At(0))->hwtriga - lasttrig);
      if (dt != 0)
      {
        gr->AddPoint(day0 + static_cast<TScalerData *>(ca->At(0))->reft, dtrig / (dt * 1.E-8));
      }
    }
    lastts = (double)(static_cast<TScalerData *>(ca->At(0))->ts);
    lasttrig = (double)(static_cast<TScalerData *>(ca->At(0))->hwtriga);
  }

  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  gr->GetXaxis()->SetNdivisions(505);

  gr->SetTitle("Trigger Rate;Time [s];Rate [pps]");
  gr->Draw("AL");
  return gr;
}

void KOBRA::ApplyOffsetToCut(Double_t xoff, Double_t yoff, Double_t xscale, Double_t yscale)
{
  for (const auto &it : cutgs)
  {
    TCutG *cut = it.second;
    for (Int_t i = 0; i < cut->GetN(); i++)
    {
      cut->GetX()[i] += xoff;
      cut->GetY()[i] += yoff;
    }
    // Scaling
    Double_t xmean = cut->GetMean(1);
    Double_t ymean = cut->GetMean(2);
    for (Int_t i = 0; i < cut->GetN(); i++)
    {
      if (xscale > 0)
        cut->GetX()[i] = xscale * (cut->GetX()[i] - xmean) + xmean;
      if (yscale > 0)
        cut->GetY()[i] = yscale * (cut->GetY()[i] - ymean) + ymean;
    }
  }
}

void KOBRA::SetCutStyle(Int_t color, Int_t width, Int_t style)
{
  for (const auto &it : cutgs)
  {
    TCutG *cut = it.second;
    cut->SetLineColor(color);
    cut->SetLineWidth(width);
    cut->SetLineStyle(style);
  }
}

TCutG *KOBRA::Make2DCut(const char *name, const char *title)
{
  std::cout << " Make a 2D cut with the name of " << name << std::endl;
  std::cout << " Please click points on the plot. " << std::endl;

  Double_t x, y, xl, yl;
  Bool_t end;

  TCutG *cut = new TCutG(name);
  cut->SetTitle(title);
  cut->SetVarX("AoQ");
  cut->SetVarY("Z");
  while (true)
  {
    xl = x;
    yl = y;
    CURRY::GetInstance()->Run(&x, &y, &end);
    cut->AddPoint(x, y);
    if (end)
      break;
  }
  cut->SetLineStyle(2);
  cut->Draw("SAME");
  return cut;
}

Double_t KOBRA::GetPPACEff(Int_t id, const char *cut, bool flagTable, std::string iso, int bias)
{
  if (flagTable)
  {
    auto eff = effF1UPPACX[iso][bias];
    if (eff > 0)
      return eff;
  }
  TString ccut = "de > 0";
  if (cut)
  {
    ccut += " && ";
    ccut += cut;
  }

  TH1 *h1;
  switch (id)
  {
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

Double_t KOBRA::GetLiveTime()
{
  std::vector<Long64_t> nEntries;

  TObjArray *fileElements = tree->GetListOfFiles();
  for (TObject *op : *fileElements)
  {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile f{chainElement->GetTitle()};
    TTree *tree = f.Get<TTree>(chainElement->GetName());
    nEntries.push_back(tree->GetEntries());
  }

  std::vector<Long64_t> nSum;
  for (auto it = nEntries.begin(); it != nEntries.end(); it++)
  {
    Int_t sum = std::accumulate(nEntries.begin(), it + 1, 0);
    nSum.push_back(sum);
  }

  Int_t totalUngated = 0;
  Int_t totalGated = 0;
  for (auto it = nSum.begin(); it != nSum.end(); it++)
  {
    Int_t n0un, n1un, n0ga, n1ga;
    if (it == nSum.begin())
    {
      for (int i = 0; i <= tree->GetEntries(); i++)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n0un = static_cast<TScalerData *>(ca->At(0))->hwtrigb;
          n0ga = static_cast<TScalerData *>(ca->At(0))->hwtriga;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n1un = static_cast<TScalerData *>(ca->At(0))->hwtrigb;
          n1ga = static_cast<TScalerData *>(ca->At(0))->hwtriga;
          break;
        }
      }
    }
    else
    {
      for (int i = *(it - 1); i <= tree->GetEntries(); i++)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n0un = static_cast<TScalerData *>(ca->At(0))->hwtrigb;
          n0ga = static_cast<TScalerData *>(ca->At(0))->hwtriga;
          break;
        }
      }
      for (int i = *it - 1; i >= 0; i--)
      {
        tree->GetEntry(i);
        if (ca->GetEntriesFast() > 0)
        {
          n1un = static_cast<TScalerData *>(ca->At(0))->hwtrigb;
          n1ga = static_cast<TScalerData *>(ca->At(0))->hwtriga;
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

void KOBRA::AnaAtOnce(const char *name, std::vector<std::string> iso)
{
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
  for (const auto &it : iso)
    yield.push_back(tree->GetEntries(it.c_str()));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // X dist.
  //////////////////////////////////////////////////////////////
  std::vector<std::vector<TH1 *>> allhists;
  std::vector<std::vector<Double_t>> allmeans;
  std::vector<std::vector<Double_t>> allsigmas;
  for (const auto &it : iso)
  {
    allhists.push_back(DrawXDist(it.c_str(), false));
    std::vector<Double_t> means;
    std::vector<Double_t> sigmas;
    for (const auto &it2 : allhists.back())
    {
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
  for (const auto &it : allhists)
  {
    for (const auto &itt : it)
    {
      cxdist->cd((i % 24) + 1);
      itt->Draw();
      i++;
    }
    if (i % 24 == 0)
    {
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
  for (size_t i = 0; i < iso.size(); i++)
  {
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

void KOBRA::PrintCutCoord(TCutG *cut, Double_t xoff, Double_t yoff)
{
  if (!cut)
    return;

  std::cout << Form("%s %s\n", cut->GetName(), cut->GetTitle());
  for (int i = 0; i < cut->GetN(); i++)
  {
    std::cout << Form("%8.5f %8.5f\n", cut->GetX()[i] + xoff, cut->GetY()[i] + yoff);
  }
}

void KOBRA::AnaAtOnceMom(const char *name, Double_t delta,  std::vector<std::string> iso)
{
  Double_t livetime;
  Double_t eff1, eff2, eff3;

  std::vector<int> yield;
  if (iso.empty())
    for (const auto it : cutgs)
      iso.push_back(it.first);

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
  for (const auto &it : iso)
    yield.push_back(tree->GetEntries(it.c_str()));
  //////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////
  // X dist.
  //////////////////////////////////////////////////////////////
  std::vector<std::vector<TH1 *>> allhists;
  std::vector<std::vector<Double_t>> allmeans;
  std::vector<std::vector<Double_t>> allsigmas;
  for (const auto &it : iso)
    {
      allhists.push_back(DrawXDist(it.c_str(), false));
      std::vector<Double_t> means;
      std::vector<Double_t> sigmas;
      for (const auto &it2 : allhists.back())
	{
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
  for (const auto &it : allhists)
    {
      for (const auto &itt : it)
	{
	  cxdist->cd((i % 24) + 1);
	  itt->Draw();
	  i++;
	}
      if (i % 24 == 0)
	{
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
  for (size_t i = 0; i < iso.size(); i++)
    {
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


void KOBRA::MomDistAnalysis()
{
  KOBRA *ko;
  ///////////////////////////////////////////////////////////////////
  // TMultiGraphs definitions
  std::vector<TMultiGraph *> mgs;
  ko = new KOBRA(KOBRA::mom00);
  for (const auto it : ko->cutgs)
  {
    auto mg = new TMultiGraph(it.first.c_str(),
                              Form("Mom. Distribution of %s;Delta [%%];Differental Rate [pps/%%]", it.second->GetTitle()));
    mgs.push_back(mg);
  }
  delete ko;
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // Loop for all settings
  std::vector<std::vector<int>> runs = {
      mom_14, mom_12, mom_10,
      mom_08, mom_07, mom_06, mom_05, mom_04,
      mom00, mom04, mom07};
  std::vector<std::string> runnames = {
      "mom_14", "mom_12", "mom_10",
      "mom_08", "mom_07", "mom_06", "mom_05", "mom_04",
      "mom00", "mom04", "mom07"};
  std::vector<Int_t> bias = {550, 550, 550, 550, 550, 575, 550, 585, 595, 595, 595};
  std::vector<Double_t>
      xoff = {0.04, 0.04, 0.035, 0.025, 0.025, 0.02, 0.015, 0.015, 0, -0.005, -0.01};
  std::vector<Double_t> yoff = {0, 0, 0, 0, 0, -0.1, -0.1, -0.1, 0, 0.1, 0.2};

  std::vector<Double_t> cens = {-14, -12, -10, -8, -7, -6, -5, -4, 0, 4, 7};
  std::vector<Double_t> bins = {0.1, 0.15, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
  std::vector<Double_t> lefts = {0, -0.05, -0.1, -0.5, -0.5, -0.5, -0.5, -1, -4, -4, -4};
  std::vector<Double_t> rights = {0.1, 0.05, 0.1, 0.5, 0.5, 0.5, 0.5, 1, 4, 4, 4};
  std::vector<bool> offs = {false, true, true, false, false, false, false, false, false, false, false};

  for (size_t j = 0; j < runs.size(); j++)
  {
    ko = new KOBRA(runs[j]);
    ko->ApplyOffsetToCut(xoff[j], yoff[j]);
    ko->AnaAtOnceMom(runnames[j].c_str(), cens[j]);
    auto grs = ko->GetMomDistGraphs(cens[j], bins[j], lefts[j], rights[j], offs[j], bias[j]);
    for (size_t i = 0; i < mgs.size(); i++)
    {
      // size_t i = 25;
      grs[i]->SetMarkerStyle(20 + j);
      grs[i]->SetMarkerColor(j + 1);
      grs[i]->SetMarkerSize(0.7);
      mgs[i]->Add(grs[i]);
    }
    delete ko;
  }
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // Artifically add
  ko = new KOBRA(o19);
  Double_t scale;
  scale = ko->GetPPACEff(1) * ko->GetPPACEff(2) * ko->GetPPACEff(3) * ko->GetLiveTime();
  auto gr = ko->GetMomDistGraph(-4, "o19", 0.5, -4, 4, false, 200 / scale);
  gr->SetMarkerStyle(20);
  gr->SetMarkerColor(1);
  gr->SetMarkerSize(0.7);
  mgs[18]->Add(gr); // 19O at -4%
  delete ko;
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Drawing
  // PDF saving
  TCanvas *cxdist = new TCanvas("cmdist", "Mom Dist", 1200, 1600);
  cxdist->Divide(3, 4);

  const int nhist = 12;
  int i = 0;
  for (const auto &it : mgs)
  {
    cxdist->cd((i % nhist) + 1);
    it->Draw("AP");
    i++;
    if (i % nhist == 0)
    {
      cxdist->Print(Form("analysis/momdist.pdf%s", i == nhist ? "(" : ""));
      cxdist->Clear("D");
    }
  }
  cxdist->Print(Form("analysis/momdist.pdf%s", i < nhist ? "" : ")"));
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Root file saving
  TFile *file = new TFile("analysis/momdist.root", "RECREATE");
  for (const auto &it : mgs)
  {
    it->Write();
  }
  file->Close();
  delete file;
  /////////////////////////////////////////////////////////////////////
}

void KOBRA::CrossSectionAnalysis()
{
  std::vector<std::vector<int>> runs = {o18, o19, o20, o21, o22};
  std::vector<std::string> runnames = {"o18", "o19", "o20", "o21", "o22"};
  std::vector<Double_t> xoff = {
      0,
      -0.005,
      -0.014,
      -0.02,
      -0.03,
  };
  std::vector<Double_t> yoff = {0, 0, 0, -0.1, 0};
  std::vector<std::vector<std::string>> isos = {o18_iso, o19_iso, o20_iso, o21_iso, o22_iso};

  KOBRA *ko;
  for (size_t j = 0; j < runs.size(); j++)
  {
    ko = new KOBRA(runs[j]);
    ko->ApplyOffsetToCut(xoff[j], yoff[j]);
    ko->AnaAtOnce(runnames[j].c_str(), isos[j]);
    delete ko;
  }
}

void KOBRA::PPACEffCurve()
{

  std::vector<Int_t> runs = {162, 163, 164, 165, 166, 167};
  std::vector<Double_t> bias = {565, 575, 585, 595, 600, 605};
  std::vector<Double_t> eff_c, eff_n, eff_o, eff_f;

  for (const auto &it : runs)
  {
    KOBRA ko(it);
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


void KOBRA::NeMomDistAnalysis(bool flagFast)
{
  KOBRA *ko;
  ///////////////////////////////////////////////////////////////////
  // TMultiGraphs definitions
  std::vector<TMultiGraph *> mgs;
  std::vector<std::string> cutiso = { "o20", "o21","f21", "f22", "f23",
				      "ne23", "ne24", "ne25", "na25", "na26", "mg27", "mg28"};
  
  for (const auto it : cutiso)
    {
      auto mg = new TMultiGraph(it.c_str(),
				Form("Mom. Distribution of %s;Delta [%%];Differental Rate [pps/%%]", it.c_str()));
      mgs.push_back(mg);
    }
  ///////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  // Loop for all settings
  std::vector<std::vector<int>> runs = {mon_10, monn_08, mon_07, mon_06, mon_05,
					mon_04, mon_03, mon_02, mon_01,
					mon00, mon01, mon02, mon04, mon06, mon10};
								  
  std::vector<std::string> runnames = {"mon_10", "monn_08", "mon_07", "mon_06", "mon_05",
				       "mon_04","mon_03", "mon_02", "mon_01",
				       "mon00", "mon01", "mon02", "mon04", "mon06", "mon10"};
  std::vector<Int_t> bias = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  std::vector<Double_t>
    xoff = {0.050, 0.037, 0.032,  0.027, 0.025, 0.025, 0.012, 0.005, 0, 0., -0.005, -0.005, -0.005, -0.01, -0.02};
  std::vector<Double_t> yoff = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::vector<Double_t> brho = {1.1768, 1.2030, 1.2161, 1.2291, 1.2422, 1.2553, 1.2684, 1.2814, 1.2945, 1.3076, 1.3207, 1.3337, 1.3599, 1.3861, 1.4384 };
  std::vector<Double_t> cens = {-10, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 4, 6, 10};
  std::vector<Double_t> bins = {0.025, 0.05, 0.1, 0.1, 0.15, 0.2, 0.2, 0.4, 0.4, 0.5, 0.4, 0.5, 0.5, 0.5, 0.5};
  std::vector<Double_t> lefts = {-0.005, -0.01, -0.04, -0.04, -0.1, -0.1,  -0.1, -0.2, -0.4, -0.5, -0.5, -1, -2, -3, -3};
  std::vector<Double_t> rights = {0.02, 0.04, 0.04, 0.04, 0.1, 0.1, 0.1, 0.2, 0.4, 0.5, 0.5, 1, 2, 3, 3};
  std::vector<bool> offs = {false, false, true, true, true, true, true, true, false, false, false, false, false, false, false};
  //  std::vector<bool> usef1 = {true, true, true, true,true,true,true,true,true,true,true,true,true };

  for (size_t j = 0; j < runs.size(); j++)
    {
      //      if (j == 2) continue;
      ko = new KOBRA(runs[j]);
      ko->SetBrho(brho[j]);
      ko->SetUseF1(true);
      ko->ApplyOffsetToCut(xoff[j], yoff[j]);
      if (!flagFast)
	ko->AnaAtOnceMom(runnames[j].c_str(), cens[j], cutiso);
      auto grs = ko->GetMomDistGraphsNe(cutiso, cens[j], bins[j], lefts[j], rights[j], offs[j], bias[j], true);
      for (size_t i = 0; i < mgs.size(); i++)
	{
	  // size_t i = 25;
	  grs[i]->SetMarkerStyle(20 + j);
	  grs[i]->SetMarkerColor(j + 1);
	  grs[i]->SetMarkerSize(0.7);
	  mgs[i]->Add(grs[i]);
	}
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
  for (const auto &it : mgs)
    {
      cxdist->cd((i % nhist) + 1);
      it->Draw("AP");
      i++;
      if (i % nhist == 0)
	{
	  //      cxdist->Print(Form("analysis/nemomdist.pdf%s", i == nhist ? "(" : ""));
	  //cxdist->Clear("D");
	}
    }
  cxdist->Print(Form("analysis/nemomdist.pdf%s", i < nhist ? "" : ")"));
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Root file saving
  TFile *file = new TFile("analysis/nemomdist.root", "RECREATE");
  for (const auto &it : mgs)
    {
      it->Write();
    }
  file->Close();
  delete file;
  /////////////////////////////////////////////////////////////////////
}
