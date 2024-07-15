#include "TCanvasManager.hxx"
#include "TDirectory.h"
#include "TROOT.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

std::vector<std::string> splitText(const std::stringstream& ss) {
    std::vector<std::string> result;
    std::regex regex(R"((\"[^\"]*\"|\S+))"); // 따옴표 안의 문자열이나 공백이 아닌 단어를 매칭
    std::string text = ss.str(); // stringstream을 string으로 변환
    auto iter = std::sregex_iterator(text.begin(), text.end(), regex);
    auto end = std::sregex_iterator();

    while (iter != end) {
        std::smatch match = *iter;
        result.push_back(match.str());
        ++iter;
    }

    return result;
}

TCanvasManager *TCanvasManager::instance = NULL;

TCanvasManager::TCanvasManager()
{
  canvases = new TClonesArray("TCanvas");
  gROOT->SetBatch();
}

TCanvasManager::~TCanvasManager()
{
    canvases->Clear("D");
    delete canvases;
}

TCanvasManager *TCanvasManager::GetInstance()
{
    if (!instance)
        instance = new TCanvasManager();
    return instance;
}

void TCanvasManager::Clear()
{
    canvases->Delete();
}

void TCanvasManager::AddCanvas(const char *name, const char *title, int xdiv, int ydiv)
{
  TCanvas *can;
    can = GetCanvas(name);
    if (can)
    {
        can->Clear();
        can->Divide(xdiv, ydiv);
        return;
    }

    can = new ((*canvases)[canvases->GetEntriesFast()]) TCanvas(name, title);
    if (xdiv > 0 && ydiv > 0)
        can->Divide(xdiv, ydiv);
}

void TCanvasManager::AddHistToCanvas(const char *name, std::vector<std::string> list)
{
    std::vector<TH1 *> h_list;
    for (const auto &s : list)
    {
        TH1 *h1 = static_cast<TH1 *>(gDirectory->FindObject(s.c_str()));
        if (h1)
            h_list.push_back(h1);
    }
    AddHistToCanvas(name, h_list);
}
void TCanvasManager::AddHistToCanvas(const char *name, std::vector<TH1 *> list)
{
    auto can = GetCanvas(name);
    if (!can)
        return;

    int i = 1;
    for (const auto &h : list)
    {
        can->cd(i++);
        h->Draw("col");
    }
}
void TCanvasManager::AddCanvasAndHistFromFile(const char *file)
{
    std::ifstream fin(file);
    if (not fin.is_open())
    {
        std::cerr << "File cannot be opened." << std::endl;
        return;
    }

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

        //std::string word;
        //std::vector<std::string> words;
        std::stringstream iss(line);
        //while (iss >> word)
        //    words.push_back(word);
        std::vector<std::string> words = splitText(iss);

        std::vector<std::string> hists;
        if (words.size() > 4)
        {
            AddCanvas(words[0].c_str(), words[1].c_str(), std::stoi(words[2]), std::stoi(words[3]));
            for (size_t j = 4; j < words.size(); j++)
                hists.push_back(words[j]);
            AddHistToCanvas(words[0].c_str(), hists);
        }
        else
            continue;
        std::cout << " Add the canvas: ";
        for (const auto &it : words)
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
    fin.close();
}

TCanvas *TCanvasManager::GetCanvas(const char *name)
{
    TCanvas *can = static_cast<TCanvas *>(canvases->FindObject(name));
    return can;
}
