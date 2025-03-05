#include "TDPlasticAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

TDPlasticAna::TDPlasticAna(const char *_name, const char *_parfile, int _n)
{
    name = _name;
    n = _n;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPlasticData", 10, 1);
    SetParameters(_parfile);
}

TDPlasticAna::~TDPlasticAna()
{
    outdata->Clear("C");
    delete outdata;
};

void TDPlasticAna::Clear()
{
    for (int i = 0; i < n; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TDPlasticAna::SetParameters(const char *file)
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

        uint32_t inum;
        float fnum;
        std::istringstream iss(line);
        switch (nline++)
        {
        case 0:
            while (iss >> inum)
                chs.push_back(inum);
            break;
        case 1:
            while (iss >> fnum)
                t_offset.push_back(fnum);
            break;
        case 2:
            while (iss >> fnum)
                t_factor.push_back(fnum);
            break;
        case 3:
            while (iss >> fnum)
                a_offset.push_back(fnum);
            break;
        case 4:
            while (iss >> fnum)
                a_factor.push_back(fnum);
            break;
        case 5:
            iss >> tdc_cut[0] >> tdc_cut[1];
        case 6:
            iss >> adc_cut[0] >> adc_cut[1];
        default:
            break;
        }
    }
    fin.close();

    flagSet = true;
}

//void TDPlasticAna::SetData(std::vector<KoBRADIGMeasurement> &_data)
void TDPlasticAna::SetData(TGenericData *rdata)
{
    if (!flagSet)
        return;

    if (!rdata) return;
    
    std::vector<KoBRADIGMeasurement> _data = static_cast<TKoBRADIGData*>(rdata)->GetMeasurements();

    for (auto it = _data.begin(); it != _data.end(); it++)
    {
        for (int i = 0; i < n; i++)
        {
            if (chs[i] == it->GetChannel() &&
                it->GetTime() > tdc_cut[0] && it->GetTime() < tdc_cut[1] &&
                it->GetEnergy() > adc_cut[0] && it->GetEnergy() < adc_cut[1])
                data[i].push_back(std::make_pair(it->GetTime(), it->GetEnergy()));
        }
    }

    flagData = true;
    for (int i = 0; i < n; i++)
        if (data[i].empty())
            flagData = false;
}

void TDPlasticAna::Analysis()
{
    if (!flagSet)
        return;
    if (!flagData)
        return;

    ////////////////////////////////////////////////////
    // single channel case
    if (n == 1)
    {
        for (auto it = data[0].begin(); it != data[0].end(); it++)
            Processing(it->first, 0, it->second, 0);

        return;
    }
    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // double channel case
    uint32_t *idx = new uint32_t[n];
    for (int i = 0; i < n; i++)
        idx[i] = 0;
    ////////////////////////////////////////////////////////////////////
    // all combinations
    while (1)
    {
        Processing(data[0][idx[0]].first, data[1][idx[1]].first, data[0][idx[0]].second, data[1][idx[1]].second);

        int next = n - 1;
        while (next >= 0 && idx[next] + 1 >= data[next].size())
            next--;

        if (next < 0)
            break;

        idx[next]++;
        for (int i = next + 1; i < n; i++)
            idx[i] = 0;
    }
    ////////////////////////////////////////////////////////////////////
}

TPlasticData *TDPlasticAna::Processing(uint32_t tl, uint32_t tr, uint32_t al, uint32_t ar)
{
    float tsum = (float(tl) + float(tr)) * t_factor[0] + t_offset[0];
    float tdiff = (float(tl) - float(tr)) * t_factor[0] + t_offset[0];
    float acl = al * a_factor[0] + a_offset[0];
    float acr = ar * a_factor[1] + a_offset[1];

    new ((*outdata)[outdata->GetEntriesFast()]) TPlasticData(tl, tr, tsum, tdiff, acl, acr);
    return static_cast<TPlasticData *>(outdata->Last());
}

void TDPlasticAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata);
}

void TDPlasticAna::PrintParameters()
{
    std::cout << "TDPlasticAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl;
    std::cout << " Channels: ";
    for (int i = 0; i < n; i++)
        std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " T Offsets: ";
    for (int i = 0; i < 2; i++)
        std::cout << t_offset[i] << " ";
    std::cout << "\n";
    std::cout << " T Factors: ";
    for (int i = 0; i < 2; i++)
        std::cout << t_factor[i] << " ";
    std::cout << "\n";
    std::cout << " A Offsets: ";
    for (int i = 0; i < 2; i++)
        std::cout << a_offset[i] << " ";
    std::cout << "\n";
    std::cout << " A Factors: ";
    for (int i = 0; i < 2; i++)
        std::cout << a_factor[i] << " ";
    std::cout << "\n";
    std::cout << " TDC cuts: " << tdc_cut[0] << " " << tdc_cut[1];
    std::cout << "\n";
    std::cout << " ADC cuts: " << adc_cut[0] << " " << adc_cut[1];
    std::cout << "\n";
}

void TDPlasticAna::PrintData()
{
    std::cout << "TDPlasticAna: " << name << std::endl;
    const char *label[2] = {"Left", "Right"};
    for (int j = 0; j < n; j++)
    {
        std::cout << " " << label[j] << ": ";
        for (int i = 0; i < int(data[j].size()); i++)
            std::cout << data[j][i].first << " " << data[j][i].second;
        std::cout << "\n";
    }
}

void TDPlasticAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TDPlasticAna: " << name << std::endl;
    TPlasticData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TPlasticData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}
