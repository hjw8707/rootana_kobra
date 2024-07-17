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

#include "TScalerData.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>


ClassImp(KOBRA);

std::vector<int> KOBRA::o18 = {69, 70, 71, 72, 73,
			       75, 76, 77, 78, 79, 80, 81, 82,
			       84, 85};
std::vector<int> KOBRA::o19 = {102, 103, 104, 105,
			       107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
			       121, 122};
std::vector<int> KOBRA::o20 = {130, 131, 132, 133, 134, 135, 136, 137,
			       147, 148,
			       150,
			       152, 153,
			       156, 157};

std::vector<int> KOBRA::o21 = {170, 171, 172,
			       180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191};


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
    for (int i = runb ; i <= rune ; i++)
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
  centBrho = 0;
  tofOff = 0;
  useF1 = false;
  useF2orF3 = false;
  useSSDorPla = true;
  // gcut = "f3ssd@.GetEntriesFast() > 0 && f3ssd@.GetEntriesFast() < 3";
  gcut = "";
  LoadDB("macros/runlog.txt");
  gStyle->SetPadGridX(true);
  gStyle->SetPadGridY(true);

  gStyle->SetCanvasDefX(2600);
  gStyle->SetCanvasDefY(100);
  //  gStyle->SetPalette(kPastel);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
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

    // Add the file to the TChain
    tree->Add(filename);
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

std::vector<std::string> KOBRA::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
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
  //SetBrho(1.356);
  SetUseF1(true);
  AddCuts("./cut");

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
  tree->SetAlias("ycor","(-0.00245304*f3uppac.y^2-0.034376*f3uppac.y+19.8065)/20");

  if (useSSDorPla) {
    if (useF2orF3)
      tree->SetAlias("de", "Max$(f2ssd.acal)");
    else
      tree->SetAlias("de", "Max$(f3ssd.acal)/ycor");
  }
  else {
    tree->SetAlias("de", "sqrt(f3dpla.al*f3dpla.ar)");
  }
  tree->SetAlias("brho", Form("(1+db/4100)*%f", centBrho));

  tree->SetAlias("beta", "12398.5/tof/299."); // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gamma", "1/sqrt(1-beta*beta)");
  tree->SetAlias("pp", "gamma*beta");

  tree->SetAlias("betap", "12398.5/tofp/299."); // F2 DPPAC - F3 UPPAC
  tree->SetAlias("gammap", "1/sqrt(1-betap*betap)");
  tree->SetAlias("ppp", "gammap*betap");

  if (useF1) {
    tree->SetAlias("aoq", "brho/(3.1*pp)");
    tree->SetAlias("aoqp", "brho/(3.1*ppp)");
  }
  else
    tree->SetAlias("aoq", "1.33/(3.1*pp)");
  tree->SetAlias("AoQ", "0.941841*aoq+0.095597");
  tree->SetAlias("AoQp", "0.941841*aoqp+0.095597");
  tree->SetAlias("corfac", "log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
  tree->SetAlias("z", "sqrt(de)*beta/sqrt(corfac)");

  if (useSSDorPla) {
    tree->SetAlias("Z", "21.540118*z+0.444610"); }
  else {
    tree->SetAlias("Z","4.8037*z-1.8");}

}

void KOBRA::AddCut(const char* filename) {

  std::ifstream file(filename);
  if (!file.is_open())
    {
      std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
      return ;
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

      std::vector<std::string> tokens = split(line, ' ');
      if (linen == 0) {
	cut = new TCutG(tokens[0].c_str());
	if (tokens.size() > 1) cut->SetTitle(tokens[1].c_str());
      }
      else {
	try {
	  cut->AddPoint(std::stof(tokens[0]), std::stof(tokens[1]));
	}
	catch (...)
	  { continue; }
      }
      linen++;
    }
  if (cut) {
    cut->SetVarX("AoQ");
    cut->SetVarY("Z");
    cut->SetFillStyle(1000);
    cutgs[cut->GetName()] = cut;}
  file.close();
}


void KOBRA::AddCuts(const char* path) {

  TSystemDirectory dir(path, path);

  TList* filesList = dir.GetListOfFiles();
  if (!filesList) {
    std::cerr << "Error: " << path << " is not a valid directory or cannot be accessed.\n";
    return;
  }

  TIter next(filesList);
  TSystemFile* file;
  while ((file = (TSystemFile*)next())) {
    TString fileName = file->GetName();
    if (!file->IsDirectory() && fileName.EndsWith("txt")) {
      AddCut(Form("%s/%s",path,fileName.Data()));
    }
  }

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
  if (show < 0 || show > 2) return;
  if (!tree)
    return;

  std::string cuts = gcut;
  if (cut)
    if (cuts.empty())
      cuts += Form("%s", cut);
    else
      cuts += Form("&& %s", cut);
  auto c1 = new TCanvas(Form("cpid_r%d",runNs.front()),"PID",1600,1200);
  tree->Draw(Form("Z:AoQ>>hpidc_r%d(400,2.1,3.1,400,4,14)",runNs.front()), cuts.c_str(), "col");

  TH2 *h2 = static_cast<TH2*>(gDirectory->Get(Form("hpidc_r%d",runNs.front())));

  std::string runstr;
  if (runNs.size() > 1)
    runstr = Form("Run %d-%d",runNs.front(),runNs.back());
  else
    runstr = Form("Run %d",runNs.front());

  const char* showcase[3] = { "", ": Count", ": Rate [pps]" };
  h2->SetTitle(Form("PID for %s%s;A/Q;Z",runstr.c_str(),showcase[show]));

  if (show > 0) {
    TLatex text;
    text.SetTextFont(62);
    text.SetTextSize(0.03);

    Double_t time;
    time = GetElapsedTime();
    for (const auto& it : cutgs) {
      TCutG* cut = it.second;
      cut->SetLineStyle(2);
      cut->Draw("SAME");
      
      Double_t x = cut->GetMean(1);
      Double_t y = cut->GetMean(2);
      TString title = cut->GetTitle();
      TString aaa = title(0,2);
      TString zzz = title(2,title.Length());
      Int_t count = GetEntries(cut->GetName());
      text.SetTextFont(62);
      text.SetTextColor(12);
      text.SetTextSize(0.03);      
      text.SetTextAlign(22);
      text.DrawLatex(x, y+0.1, Form("^{%s}%s",aaa.Data(),zzz.Data()));

      text.SetTextFont(52);      
      text.SetTextColor(12);
      text.SetTextSize(0.03);
      text.SetTextAlign(12);
      TString a;

      if (show == 1) a = Form("%d",count);
      else           a = Form("%.4f", float(count/time));
      
      text.DrawLatex(x+0.005, y-0.3, a.Data());
    }
    std::string timestr;
    Int_t itime = time;
    if (itime > 3600)
      timestr = Form("%02d:%02d:%02d", itime/3600, (itime%3600)/60, itime%60);
    else if (itime > 60)
      timestr = Form("%02d:%02d", itime/60, itime%60);
    else
      timestr = "";
      
    text.DrawLatexNDC(0.15,0.92,Form("Brho: %.4f Tm / Time: %.0f sec. or %s",GetBrho(),time, timestr.c_str()));
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

void KOBRA::CountIsotopes() {
  ////////////////////////////////////////////////////////////
  // Counting
  std::cout << "=========================================" << std::endl;
  std::cout << "   Counts of Isotopes " << std::endl;
  std::cout << "=========================================" << std::endl;
  for (const auto& it : cutgs) {
    std::cout << " ";
    std::cout << it.second->GetTitle();
    std::cout << ": ";
   std::cout << GetEntries(it.second->GetName());
    std::cout << std::endl;
  }
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////

}

void KOBRA::DrawAllCut() {

  for (const auto &it: cutgs) {
    it.second->Draw("SAME");
  }
}


void KOBRA::DrawXDist(const char *cut) {
  auto c1 = new TCanvas(Form("cxd_r%d_%s",runNs.front(),cut),"X Distribution",1200,400);
  c1->Divide(3,1);

  c1->cd(1);
  tree->Draw(Form("f1x>>hf1x_r%d_%s(100,-200,200)",runNs.front(),cut),cut);

  c1->cd(2);
  tree->Draw(Form("f2x>>hf2x_r%d_%s(100,-200,200)",runNs.front(),cut),cut);
  
  c1->cd(3);
  tree->Draw(Form("f3x>>hf3x_r%d_%s(100,-200,200)",runNs.front(),cut),cut);
  

}

Double_t KOBRA::GetElapsedTime() {

  std::vector<Long64_t> nEntries;
  
  TObjArray *fileElements=tree->GetListOfFiles();
  for (TObject *op: *fileElements) {
    auto chainElement = static_cast<TChainElement *>(op);
    TFile f{chainElement->GetTitle()};
    TTree *tree = f.Get<TTree>(chainElement->GetName());
    nEntries.push_back(tree->GetEntries());
  }

  std::vector<Long64_t> nSum;
  for (auto it = nEntries.begin() ; it != nEntries.end() ; it++)  {
    Int_t sum = std::accumulate(nEntries.begin(), it+1,0);
    nSum.push_back(sum); }

  Double_t totalTime = 0;  
  for (auto it = nSum.begin() ; it != nSum.end() ; it++) {
    Int_t n0, n1;
    if (it == nSum.begin()) {
      for (int i = 0 ; i <= tree->GetEntries() ; i++) {
	tree->GetEntry(i);
	if (ca->GetEntriesFast() > 0) {
	  n0 = static_cast<TScalerData*>(ca->At(0))->reft;
	  break; }}
      for (int i = *it - 1 ; i >= 0 ; i--) {
	tree->GetEntry(i);
	if (ca->GetEntriesFast() > 0) {
	  n1 = static_cast<TScalerData*>(ca->At(0))->reft;
	  break; }}}
    else {
      for (int i = *(it-1) ; i <= tree->GetEntries() ; i++) {
	tree->GetEntry(i);
	if (ca->GetEntriesFast() > 0) {
	  n0 = static_cast<TScalerData*>(ca->At(0))->reft;
	  break; }}      
      for (int i = *it - 1 ; i >= 0 ; i--) {
	tree->GetEntry(i);
	if (ca->GetEntriesFast() > 0) {
	  n1 = static_cast<TScalerData*>(ca->At(0))->reft;
	  break; }}}
    totalTime += (Double_t)(n1 - n0);}
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

void KOBRA::DrawPPACEff(const char *cut){
  auto c1 = new TCanvas(Form("cpeff_r%d",runNs.front()),"PPAC Eff.",800,1200);
  c1->Divide(2,3);

  TH1 *h1;
  c1->cd(1);
  tree->Draw(Form("f1uppac@.GetEntriesFast()>>hf1up_r%d(2,0,2)",runNs.front()),cut);
  h1 = static_cast<TH1*>(gDirectory->Get(Form("hf1up_r%d",runNs.front())));
  std::cout << " F1 Up PPAC Eff. : " << h1->GetMean() << std::endl;
  
  c1->cd(2);
  tree->Draw(Form("f1uppacx@.GetEntriesFast()>>hf1upx_r%d(2,0,2)",runNs.front()),cut);  
  h1 = static_cast<TH1*>(gDirectory->Get(Form("hf1upx_r%d",runNs.front())));
  std::cout << " F1 Up PPACX Eff.: " << h1->GetMean() << std::endl;
  
  c1->cd(4);
  tree->Draw(Form("f2dppac@.GetEntriesFast()>>hf2dp_r%d(2,0,2)",runNs.front()),cut);
  h1 = static_cast<TH1*>(gDirectory->Get(Form("hf2dp_r%d",runNs.front())));
  std::cout << " F2 Dn PPAC Eff. : " << h1->GetMean() << std::endl;
  
  c1->cd(5);
  tree->Draw(Form("f3uppac@.GetEntriesFast()>>hf3up_r%d(2,0,2)",runNs.front()),cut);
  h1 = static_cast<TH1*>(gDirectory->Get(Form("hf3up_r%d",runNs.front())));
  std::cout << " F3 Up PPAC Eff. : " << h1->GetMean() << std::endl;
  
  c1->cd(6);
  tree->Draw(Form("f3dppac@.GetEntriesFast()>>hf3dp_r%d(2,0,2)",runNs.front()),cut);  
  h1 = static_cast<TH1*>(gDirectory->Get(Form("hf3dp_r%d",runNs.front())));
  std::cout << " F3 Dn PPAC Eff. : " << h1->GetMean() << std::endl;
}

TGraph* KOBRA::PPACRate() {

  TGraph *gr = new TGraph;

  Double_t lastts = -1;
  Double_t lastppac = -1;
  for (Int_t i = 0 ; i < tree->GetEntries() ; i+=10) {
    tree->GetEntry(i);
    if (!ca || ca->GetEntriesFast() == 0) continue;
    if (i > 1) {
      Double_t dt = (double)(static_cast<TScalerData*>(ca->At(0))->ts - lastts);
      Double_t dppac = (double)( static_cast<TScalerData*>(ca->At(0))->ppac - lastppac);
      if (dt != 0) {
	gr->AddPoint(static_cast<TScalerData*>(ca->At(0))->reft, dppac/(dt*1.E-8));}
    }
    lastts = (double)(static_cast<TScalerData*>(ca->At(0))->ts);
    lastppac = (double)(static_cast<TScalerData*>(ca->At(0))->ppac);
  }

  gr->GetXaxis()->SetTimeDisplay(1);
  gr->GetXaxis()->SetTimeFormat("%H:%M:%S%F2024-07-15 15:00:00");
  
  gr->SetTitle("PPAC Or Rate;Time [s];Rate [pps]");
  gr->Draw("AL");
  return gr;
}

void KOBRA::ApplyOffsetToCut(Double_t xoff, Double_t yoff) {
  for (const auto& it : cutgs) {
    TCutG* cut = it.second;
    for (Int_t i = 0 ; i < cut->GetN() ; i++) {
      cut->GetX()[i] += xoff;
      cut->GetY()[i] += yoff;
    }}
}

TCutG* KOBRA::Make2DCut(const char* name, const char* title){
  std::cout << " Make a 2D cut with the name of " << name << std::endl;
  std::cout << " Please click points on the plot. " << std::endl;

  Double_t x, y, xl, yl;
  Bool_t end;

  TCutG *cut = new TCutG(name);
  cut->SetTitle(title);
  cut->SetVarX("AoQ");
  cut->SetVarY("Z");
  while (true) {
    xl = x; yl = y;
    CURRY::GetInstance()->Run(&x, &y, &end);
    cut->AddPoint(x, y);
    if (end) break;    
  }
  cut->SetLineStyle(2);
  cut->Draw("SAME");
  return cut;
}
