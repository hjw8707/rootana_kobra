#include "TPlasticAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

TPlasticAna::TPlasticAna(const char *_name, const char *_parfile, int _n)
{
    name = _name;
    n = _n;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPlasticData", 10, 1);
    SetParameters(_parfile);
}

TPlasticAna::~TPlasticAna()
{
    outdata->Clear("C");
    delete outdata;
};

void TPlasticAna::Clear()
{
    for (int i = 0; i < n; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TPlasticAna::SetParameters(const char *file)
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
                offset.push_back(fnum);
            break;
        case 2:
            while (iss >> fnum)
                factor.push_back(fnum);
            break;
        case 3:
            iss >> tdc_cut[0] >> tdc_cut[1];
        default:
            break;
        }
    }
    fin.close();

    flagSet = true;
}

//void TPlasticAna::SetData(std::vector<KoBRATDCMeasurement> &_data)
void TPlasticAna::SetData(TGenericData *rdata)
{
    if (!flagSet)
        return;

    if (!rdata)
        return;

    std::vector<KoBRATDCMeasurement> _data =
        static_cast<TKoBRATDCData *>(rdata)->GetMeasurements();

    for (auto it = _data.begin(); it != _data.end(); it++)
    {
        for (int i = 0; i < n; i++)
        {
            if (chs[i] == it->GetChannel() &&
                it->GetMeasurement() > tdc_cut[0] &&
                it->GetMeasurement() < tdc_cut[1])
                data[i].push_back(it->GetMeasurement());
        }
    }

    flagData = true;
    for (int i = 0; i < n; i++)
        if (data[i].empty())
            flagData = false;
}

void TPlasticAna::Analysis()
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
            Processing(*it, 0);

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
        Processing(data[0][idx[0]], data[1][idx[1]]);

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

TPlasticData *TPlasticAna::Processing(uint32_t tl, uint32_t tr)
{
    float tsum = (float(tl) + float(tr)) * factor[0] + offset[0];
    float tdiff = (float(tl) - float(tr)) * factor[0] + offset[0];

    new ((*outdata)[outdata->GetEntriesFast()]) TPlasticData(tl, tr, tsum, tdiff);
    return static_cast<TPlasticData *>(outdata->Last());
}

void TPlasticAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata);
    // tree->Branch(Form("%sppac", name.c_str()), &flagSet, "flagSet/B");
}

void TPlasticAna::PrintParameters()
{
    std::cout << "TPlasticAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl;
    std::cout << " Channels: ";
    for (int i = 0; i < n; i++)
        std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " Offsets: ";
    for (int i = 0; i < 2; i++)
        std::cout << offset[i] << " ";
    std::cout << "\n";
    std::cout << " Factors: ";
    for (int i = 0; i < 2; i++)
        std::cout << factor[i] << " ";
    std::cout << "\n";
    std::cout << " TDC cuts: " << tdc_cut[0] << " " << tdc_cut[1];
    std::cout << "\n";
}

void TPlasticAna::PrintData()
{
    std::cout << "TPlasticAna: " << name << std::endl;
    const char *label[n] = {"Left", "Right"};
    for (int j = 0; j < n; j++)
    {
        std::cout << " " << label[j] << ": ";
        for (int i = 0; i < int(data[j].size()); i++)
            std::cout << data[j][i] << " ";
        std::cout << "\n";
    }
}

void TPlasticAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TPlasticAna: " << name << std::endl;
    TPlasticData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TPlasticData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}