#include "TSSDAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

TSSDAna::TSSDAna(const char *_name, const char *_parfile, int _n)
{
    name = _name;
    n = _n;
    flagSet = false;
    flagData = false;

    for (int i = 0; i < n; i++)
    {
        std::vector<uint32_t> temp;
        data.push_back(temp);
    }

    outdata = new TClonesArray("TSSDData", 10);
    SetParameters(_parfile);
}

TSSDAna::~TSSDAna()
{
    outdata->Clear("C");
    delete outdata;
}

void TSSDAna::Clear()
{
    for (int i = 0; i < n; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TSSDAna::SetParameters(const char *file)
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
                ped.push_back(fnum);
            break;
        case 2:
            while (iss >> fnum)
                offset.push_back(fnum);
            break;
        case 3:
            while (iss >> fnum)
                factor.push_back(fnum);
            break;
        case 4:
            iss >> adc_cut[0] >> adc_cut[1];
        default:
            break;
        }
    }
    fin.close();

    flagSet = true;
}

void TSSDAna::SetData(TGenericData *rdata)
{
    if (!flagSet)
        return;

    if (!rdata) return;
    
    std::vector<KoBRAADCMeasurement> _data = static_cast<TKoBRAADCData*>(rdata)->GetMeasurements();


    for (auto it = _data.begin(); it != _data.end(); it++)
    {
        for (int i = 0; i < n; i++)
        {
            if (chs[i] == it->GetChannel() &&
                it->GetMeasurement() > adc_cut[0] &&
                it->GetMeasurement() < adc_cut[1])
                data[i].push_back(it->GetMeasurement());
        }
    }

    flagData = false;
    for (int i = 0; i < n; i++)
        if (!data[i].empty())
            flagData = true;
}

void TSSDAna::Analysis()
{
    if (!flagSet)
        return;
    if (!flagData)
        return;

    ////////////////////////////////////////////////////////////////////
    // put each data
    for (int i = 0; i < n; i++)
    { // for each channel
        for (auto it = data[i].begin(); it != data[i].end(); it++)
        {
            Processing(i, *it);
        }
    }
    ////////////////////////////////////////////////////////////////////    
}

TSSDData *TSSDAna::Processing(int ch, uint32_t adc)
{
    new ((*outdata)[outdata->GetEntriesFast()]) TSSDData(ch, adc, (float(adc) - ped[ch]) * factor[ch] + offset[ch]);
    return static_cast<TSSDData *>(outdata->Last());
}

void TSSDAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%sssd", name.c_str()), &outdata);
}

void TSSDAna::PrintParameters()
{
    std::cout << "TSSDAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl;
    std::cout << " Channels: ";
    for (int i = 0; i < n; i++)
        std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " Pedestal: ";
    for (int i = 0; i < n; i++)
        std::cout << ped[i] << " ";
    std::cout << "\n";
    std::cout << " Offsets: ";
    for (int i = 0; i < n; i++)
        std::cout << offset[i] << " ";
    std::cout << "\n";
    std::cout << " Factors: ";
    for (int i = 0; i < n; i++)
        std::cout << factor[i] << " ";
    std::cout << "\n";
    std::cout << " ADC cuts: " << adc_cut[0] << " " << adc_cut[1];
    std::cout << "\n";
}

void TSSDAna::PrintData()
{
    std::cout << "TSSDAna: " << name << std::endl;
    for (int j = 0; j < n; j++)
    {
        std::cout << " CH " << j << ": ";
        for (int i = 0; i < int(data[j].size()); i++)
            std::cout << data[j][i] << " ";
        std::cout << "\n";
    }
}

void TSSDAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TSSDAna: " << name << std::endl;
    TSSDData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TSSDData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}