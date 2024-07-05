#include "TPPACAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

TPPACAna::TPPACAna(const char *_name, const char *_parfile)
{
    name = _name;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPPACData", 10);
    SetParameters(_parfile);
}

TPPACAna::~TPPACAna()
{
    outdata->Clear("C");
    delete outdata;
};

void TPPACAna::Clear()
{
    for (int i = 0; i < n; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TPPACAna::SetParameters(const char *file)
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

// void TPPACAna::SetData(std::vector<KoBRATDCMeasurement>& _data) {
void TPPACAna::SetData(TGenericData *rdata)
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

void TPPACAna::Analysis()
{
    if (!flagSet)
        return;
    if (!flagData)
        return;

    uint32_t *idx = new uint32_t[n];
    for (int i = 0; i < n; i++)
        idx[i] = 0;

    ////////////////////////////////////////////////////////////////////
    // all combinations
    while (1)
    {
        Processing(data[0][idx[0]], data[1][idx[1]],
                   data[2][idx[2]], data[3][idx[3]], data[4][idx[4]]);

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

TPPACData *TPPACAna::Processing(uint32_t tx1, uint32_t tx2,
                                uint32_t ty1, uint32_t ty2,
                                uint32_t ta)
{
    float x = (float(tx1) - float(tx2)) * factor[0] + offset[0];
    float y = (float(ty1) - float(ty2)) * factor[1] + offset[1];

    new ((*outdata)[outdata->GetEntriesFast()]) TPPACData(tx1, tx2, ty1, ty2, ta, x, y);
    return static_cast<TPPACData *>(outdata->Last());
}

void TPPACAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata);
    // tree->Branch(Form("%sppac", name.c_str()), &flagSet, "flagSet/B");
}

void TPPACAna::PrintParameters()
{
    std::cout << "TPPACAna: " << name << std::endl;
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

void TPPACAna::PrintData()
{
    std::cout << "TPPACAna: " << name << std::endl;
    const char *label[n] = {"X_UP", "X_DN", "Y_UP", "Y_DN", "Anode"};
    for (int j = 0; j < n; j++)
    {
        std::cout << " " << label[j] << ": ";
        for (int i = 0; i < int(data[j].size()); i++)
            std::cout << data[j][i] << " ";
        std::cout << "\n";
    }
}

void TPPACAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TPPACAna: " << name << std::endl;
    TPPACData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TPPACData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}