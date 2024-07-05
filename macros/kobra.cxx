#include "kobra.hxx"

#include "TFile.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

ClassImp(KOBRA);

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
    RunSetting(run);
}
KOBRA::KOBRA(std::vector<int> runs)
{
    Initilize();
    LoadTree(runs);
    RunSetting(runs[0]);
}
KOBRA::KOBRA(int runb, int rune)
{
    Initilize();
    LoadTree(runb, rune);
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
    runN = 0;
    centBrho = 0;
    tofOff = 0;
    useF1 = false;
    useF2orF3 = false;
    gcut = "f3ssd@.GetEntriesFast() > 0 && f3ssd@.GetEntriesFast() < 3";
    LoadDB("macros/runlog.txt");
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
            brho = std::stof(tokens[8]);
        }
        catch (...)
        {
            continue;
        }
        brhoMap[run] = brho;
    }

    file.close();
    return true;
}

void KOBRA::RunSetting(int run)
{
    runN = run;
    try
    {
        SetBrho(brhoMap[runN]);
    }
    catch (...)
    {
    }
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

    tree->SetAlias("db", "f1uppac.x");
    tree->SetAlias("tof", Form("(f3uppt.tsum - f2dppt.tsum) + 103.8 + %f", tofOff));
    //  tree->SetAlias("de", "Max$(f2ssd.acal)");
    if (useF2orF3)
        tree->SetAlias("de", "Max$(f2ssd.acal)");
    else
        tree->SetAlias("de", "Max$(f3ssd.acal)");

    tree->SetAlias("brho", Form("(1+db/4100)*%f", centBrho));

    tree->SetAlias("beta", "12398.5/tof/299."); // F2 DPPAC - F3 UPPAC
    tree->SetAlias("gamma", "1/sqrt(1-beta*beta)");
    tree->SetAlias("pp", "gamma*beta");

    if (useF1)
        tree->SetAlias("aoq", "brho/(3.1*pp)");
    else
        tree->SetAlias("aoq", "1.33/(3.1*pp)");
    tree->SetAlias("AoQ", "0.951339*aoq+0.088494");
    tree->SetAlias("corfac", "log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
    tree->SetAlias("z", "sqrt(de)*beta/sqrt(corfac)");
    tree->SetAlias("Z", "21.540118*z+0.444610");
}

void KOBRA::DrawPID0(const char *cut)
{
    if (!tree)
        return;

    std::string cuts = gcut;
    if (cut)
        cuts += Form("&& %s", cut);
    tree->Draw("de:tof>>hpid0(400,150,350,400,0,150)", cuts.c_str(), "col");
}

void KOBRA::DrawPID(const char *cut)
{
    if (!tree)
        return;

    std::string cuts = gcut;
    if (cut)
        cuts += Form("&& %s", cut);
    tree->Draw("z:aoq>>hpid(400,1.5,3,400,0,1)", cuts.c_str(), "col");
}

void KOBRA::DrawPIDC(const char *cut)
{
    if (!tree)
        return;

    std::string cuts = gcut;
    if (cut)
        cuts += Form("&& %s", cut);
    tree->Draw("Z:AoQ>>hpidc(400,1.5,3,400,0,20)", cuts.c_str(), "col");
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