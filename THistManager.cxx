#include "THistManager.hxx"
#include "TDirectory.h"
#include "TTreeManager.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

THistManager *THistManager::instance = NULL;

THistManager::THistManager() : flagBlock(true)
{
}

THistManager::~THistManager()
{
}

THistManager *THistManager::GetInstance()
{
    if (!instance)
        instance = new THistManager();
    return instance;
}

void THistManager::AddHistFromFile(const char *file)
{
    std::ifstream fin(file);
    if (not fin.is_open())
    {
        std::cerr << "File cannot be opened." << std::endl;
        return;
    }

    TDirectory *curDir = gDirectory;
    if (!curDir->cd("hists"))
    {
        curDir->mkdir("hists", "hists");
        curDir->cd("hists");
    }

    TTree *tree = TTreeManager::GetInstance()->GetTree();

    std::string line;
    while (std::getline(fin, line))
    {
        // #으로 시작하는 주석을 제거
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        // 공백만 있는 줄은 무시
        if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
            continue;

        std::string word;
        std::vector<std::string> words;
        std::istringstream iss(line);
        while (iss >> word)
            words.push_back(word);

        std::vector<std::string> hists;
        if (words.size() >= 10)
        {                                            // for 2D ( 10: no cut, 11: cut)
            TH2 *h2 = new TH2D(words[0].c_str(),     // name
                               words[1].c_str(),     // title
                               std::stoi(words[3]),  // x bins
                               std::stof(words[4]),  // x low lim
                               std::stof(words[5]),  // x upp lim
                               std::stoi(words[7]),  // y bins
                               std::stof(words[8]),  // y low lim
                               std::stof(words[9])); // y upp lim
            Hist2D.push_back(h2);
            tf2d_x.push_back(new TTreeFormula(Form("tf2dx_%s", words[0].c_str()),
                                              words[2].c_str(),
                                              tree));
            tf2d_y.push_back(new TTreeFormula(Form("tf2dy_%s", words[0].c_str()),
                                              words[6].c_str(),
                                              tree));
            if (words.size() > 10)
                tf2d_c.push_back(new TTreeFormula(Form("tf2dc_%s", words[0].c_str()),
                                                  words[10].c_str(),
                                                  tree));
            else
                tf2d_c.push_back(NULL);
        }
        else if (words.size() >= 6)
        {
            TH1 *h1 = new TH1D(words[0].c_str(),     // name
                               words[1].c_str(),     // title
                               std::stoi(words[3]),  // x bins
                               std::stof(words[4]),  // x low lim
                               std::stof(words[5])); // x upp lim
            Hist1D.push_back(h1);
            tf1d_x.push_back(new TTreeFormula(Form("tf1dx_%s", words[0].c_str()),
                                              words[2].c_str(),
                                              tree));
            if (words.size() > 6)
                tf1d_c.push_back(new TTreeFormula(Form("tf1dc_%s", words[0].c_str()),
                                                  words[6].c_str(),
                                                  tree));
            else
                tf1d_c.push_back(NULL);
        }
        else
            continue;
        std::cout << " Add the Hist: ";
        for (const auto &it : words)
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
    fin.close();
    curDir->cd();
    flagBlock = false;
}

void THistManager::Clear()
{
    flagBlock = true;

    for (const auto &h1 : Hist1D)
        if (h1)
            delete h1;
    Hist1D.clear();

    for (const auto &h2 : Hist2D)
        if (h2)
            delete h2;
    Hist2D.clear();

    for (const auto &tf1 : tf1d_x)
        if (tf1)
            delete tf1;
    tf1d_x.clear();

    for (const auto &tf1 : tf1d_c)
        if (tf1)
            delete tf1;
    tf1d_c.clear();

    for (const auto &h2 : tf2d_x)
        if (h2)
            delete h2;
    tf2d_x.clear();
    for (const auto &h2 : tf2d_y)
        if (h2)
            delete h2;
    tf2d_y.clear();
    for (const auto &h2 : tf2d_c)
        if (h2)
            delete h2;
    tf2d_c.clear();
}

void THistManager::Fill()
{
    if (flagBlock)
        return;
    //TTreeManager::GetInstance()->GetTree()->Show();
    for (size_t i = 0; i < Hist1D.size(); i++)
    {
        TH1 *h1 = Hist1D[i];
        if (h1 && (!tf1d_c[i] || tf1d_c[i]->EvalInstance()))
            for (size_t j = 0; j < tf1d_x[i]->GetNdata(); j++)
                h1->Fill(tf1d_x[i]->EvalInstance(j));
    }

    for (size_t i = 0; i < Hist2D.size(); i++)
    {
        TH2 *h2 = Hist2D[i];
        if (h2 && (!tf2d_c[i] || tf2d_c[i]->EvalInstance()))
        {
            for (size_t j = 0; j < tf2d_x[i]->GetNdata(); j++)
                for (size_t k = 0; k < tf2d_y[i]->GetNdata(); k++)
                    h2->Fill(tf2d_x[i]->EvalInstance(j), tf2d_y[i]->EvalInstance(k));
        }
    }
}

void THistManager::Reset()
{
    for (const auto &h1 : Hist1D)
    {
        h1->Reset();
    }

    for (const auto &h2 : Hist2D)
    {
        h2->Reset();
    }
}