#include "THistManager.hxx"
#include "TDirectory.h"
#include "TTreeManager.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

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
    std::regex re(R"((\"[^\"]*\")|([+-]?\d+(\.\d+)?([eE][+-]?\d+)?))"); // 문자열 또는 숫자를 추출하기 위한 정규 표현식
    while (std::getline(fin, line))
    {
        // #으로 시작하는 주석을 제거
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        // 공백만 있는 줄은 무시
        if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
            continue;

        std::vector<std::string> words;
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), re);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        {
            std::smatch match = *i;
            std::string match_str = match.str();
            if (!match_str.empty())
            {
                if (match_str.front() == '"' && match_str.back() == '"')
                {
                    words.push_back(match_str.substr(1, match_str.size() - 2));
                }
                else
                {
                    words.push_back(match_str);
                }
            }
        }

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
            Hist2D[words[0]] = h2;
            tf2d_x[words[0]] = new TTreeFormula(Form("tf2dx_%s", words[0].c_str()),
                                                words[2].c_str(),
                                                tree);
            tf2d_y[words[0]] = new TTreeFormula(Form("tf2dy_%s", words[0].c_str()),
                                                words[6].c_str(),
                                                tree);
            if (words.size() > 10)
                tf2d_c[words[0]] = new TTreeFormula(Form("tf2dc_%s", words[0].c_str()),
                                                    words[10].c_str(),
                                                    tree);
            else
                tf2d_c[words[0]] = NULL;
        }
        else if (words.size() >= 6)
        {
            TH1 *h1 = new TH1D(words[0].c_str(),     // name
                               words[1].c_str(),     // title
                               std::stoi(words[3]),  // x bins
                               std::stof(words[4]),  // x low lim
                               std::stof(words[5])); // x upp lim
            Hist1D[words[0]] = h1;
            tf1d_x[words[0]] = new TTreeFormula(Form("tf1dx_%s", words[0].c_str()),
                                              words[2].c_str(),
                                              tree);
            if (words.size() > 6)
                tf1d_c[words[0]] = new TTreeFormula(Form("tf1dc_%s", words[0].c_str()),
                                                  words[6].c_str(),
                                                  tree);
            else
                tf1d_c[words[0]] = NULL;
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

    for (const auto &n : Hist1D)
        if (n.second)
            delete n.second;
    Hist1D.clear();

    for (const auto &n : Hist2D)
        if (n.second)
            delete n.second;
    Hist2D.clear();

    for (const auto &tf1 : tf1d_x)
        if (tf1.second)
            delete tf1.second;
    tf1d_x.clear();

    for (const auto &tf1 : tf1d_c)
        if (tf1.second)
            delete tf1.second;
    tf1d_c.clear();

    for (const auto &h2 : tf2d_x)
        if (h2.second)
            delete h2.second;
    tf2d_x.clear();
    for (const auto &h2 : tf2d_y)
        if (h2.second)
            delete h2.second;
    tf2d_y.clear();
    for (const auto &h2 : tf2d_c)
        if (h2.second)
            delete h2.second;
    tf2d_c.clear();
}

void THistManager::Fill()
{
    if (flagBlock)
        return;

    for (const auto &n : Hist1D)
    {
        if (n.second && (!tf1d_c[n.first] || tf1d_c[n.first]->EvalInstance()))
            for (size_t j = 0; j < tf1d_x[n.first]->GetNdata(); j++)
                n.second->Fill(tf1d_x[n.first]->EvalInstance(j));
    }

    for (const auto &n : Hist2D)
    {
        if (n.second && (!tf2d_c[n.first] || tf2d_c[n.first]->EvalInstance()))
        {
            for (size_t j = 0; j < tf2d_x[n.first]->GetNdata(); j++)
                for (size_t k = 0; k < tf2d_y[n.first]->GetNdata(); k++)
                    n.second->Fill(tf2d_x[n.first]->EvalInstance(j), tf2d_y[n.first]->EvalInstance(k));
        }
    }
}

void THistManager::Reset()
{
    for (const auto &n : Hist1D)
    {
        n.second->Reset();
    }

    for (const auto &n : Hist2D)
    {
        n.second->Reset();
    }
}