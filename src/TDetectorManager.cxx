#include "TDetectorManager.hxx"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TPPACAna.hxx"
#include "TLPPACAna.hxx"
#include "TLPPACXAna.hxx"
#include "TPlasticAna.hxx"
#include "TDPlasticAna.hxx"
#include "TSSDAna.hxx"

#include "TKoBRATDCData.hxx"
#include "TKoBRAADCData.hxx"
#include "TKoBRADIGData.hxx"

TDetectorManager *TDetectorManager::instance = NULL;

TDetectorManager::TDetectorManager()
{
    dtype_map["KoBRATDC"] = KoBRATDC;
    dtype_map["KoBRAADC"] = KoBRAADC;
    dtype_map["KoBRADIG"] = KoBRADIG;

    atype_map["PPAC"] = PPAC;
    atype_map["LPPAC"] = LPPAC;
    atype_map["LPPACX"] = LPPACX;
    atype_map["Plastic"] = Plastic;
    atype_map["DPlastic"] = DPlastic;
    atype_map["SSD"] = SSD;
}

void TDetectorManager::AddDetectorAna(const char *name, DTYPE dtype, const char *bank, TDetectorAna *ana)
{
    if (anas.find(name) == anas.end())
    {
        anas[name] = ana;
        dtypes[name] = dtype;
        banks[name] = bank;
    }
}

void TDetectorManager::AddDetectorAna(const char *name, DTYPE dtype, const char *bank, ATYPE atype, const char *par, int n)
{
    if (anas.find(name) == anas.end())
    {
        TDetectorAna *ana;
        switch (atype)
        {
        case PPAC:
            ana = new TPPACAna(name, par);
            break;
        case LPPAC:
            ana = new TLPPACAna(name, par);
            break;
        case LPPACX:
            ana = new TLPPACXAna(name, par);
            break;
        case Plastic:
            ana = new TPlasticAna(name, par, n);
            break;
        case DPlastic:
            ana = new TDPlasticAna(name, par, n);
            break;
        case SSD:
            ana = new TSSDAna(name, par, n);
            break;
        default:
            break;
        }
        anas[name] = ana;
        dtypes[name] = dtype;
        banks[name] = bank;
    }
}

TDetectorAna *TDetectorManager::GetDetectorAna(const char *name)
{
    auto it = anas.find(name);
    if (it == anas.end())
        return NULL;
    else
        return it->second;
}

void TDetectorManager::LoadAnaList(const char *file)
{
    std::ifstream fin(file);
    if (not fin.is_open())
    {
        std::cerr << "File cannot be opened." << std::endl;
        return;
    }

    int nline = 0;
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

        if (words.size() == 5)
            AddDetectorAna(words[0].c_str(), dtype_map[words[1]], words[2].c_str(), atype_map[words[3]], words[4].c_str());
        else
            AddDetectorAna(words[0].c_str(), dtype_map[words[1]], words[2].c_str(), atype_map[words[3]], words[4].c_str(), std::stoi(words[5]));
        std::cout << " Add the analysis: ";
        for (const auto &it : words)
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }

    fin.close();
}

void TDetectorManager::PrintParametersAll()
{
    for (const auto &pair : anas)
    {
        pair.second->PrintParameters();
    }
}

void TDetectorManager::SetTreeAll()
{
    for (const auto &pair : anas)
    {
        pair.second->SetTree();
    }
}

void TDetectorManager::SetDataAll(TDataContainer &container)
{
    for (const auto &pair : anas)
    {
        switch (dtypes[pair.first])
        {
        case KoBRATDC:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRATDCData>(banks[pair.first].c_str())));
            break;
        case KoBRAADC:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRAADCData>(banks[pair.first].c_str())));
            break;
        case KoBRADIG:
            pair.second->SetData(static_cast<TGenericData *>(container.GetEventData<TKoBRADIGData>(banks[pair.first].c_str())));
            break;
        default:
            break;
        }
    }
}

void TDetectorManager::ClearAll()
{
    for (const auto &pair : anas)
    {
        pair.second->Clear();
    }
}

void TDetectorManager::AnalysisAll()
{
    for (const auto &pair : anas)
    {
        pair.second->Analysis();
    }
}

TDetectorManager *TDetectorManager::GetInstance()
{
    if (!instance)
        instance = new TDetectorManager();
    return instance;
}
