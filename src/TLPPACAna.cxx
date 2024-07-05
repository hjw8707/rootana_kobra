#include "TLPPACAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

TLPPACAna::TLPPACAna(const char *_name, const char *_parfile)
{
    name = _name;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPPACData", 10, 1);
    SetParameters(_parfile);
}

TLPPACAna::~TLPPACAna()
{
    outdata->Clear("C");
    delete outdata;
};

void TLPPACAna::Clear()
{
    for (int i = 0; i < n; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TLPPACAna::SetParameters(const char *file)
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

// void TLPPACAna::SetData(std::vector<KoBRATDCMeasurement> &_data)
void TLPPACAna::SetData(TGenericData *rdata)
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
    for (int i = 6; i < n; i++) // at least yup, ydn, anode
        if (data[i].empty())
            flagData = false;
    if ((data[0].empty() || data[1].empty()) &&
        (data[2].empty() || data[3].empty()) &&
        (data[4].empty() || data[5].empty()))
        flagData = false;
}

void TLPPACAna::Analysis()
{
    if (!flagSet)
        return;
    if (!flagData)
        return;

    const int m = 5;
    uint32_t *idx = new uint32_t[m];
    std::vector<uint32_t> arr[m];
    arr[2] = data[6];
    arr[3] = data[7];
    arr[4] = data[8];
    for (int i = 0; i < 3; i++)
    {
        if (data[2 * i].empty() || data[2 * i + 1].empty())
            continue;

        arr[0] = data[2 * i];
        arr[1] = data[2 * i + 1];
        for (int j = 0; j < m; j++)
            idx[j] = 0;

        // all combinations
        while (1)
        {
            Processing(arr[0][idx[0]], arr[1][idx[1]],
                       arr[2][idx[2]], arr[3][idx[3]], arr[4][idx[4]], i);
            int next = m - 1;
            while (next >= 0 && idx[next] + 1 >= data[next].size())
                next--;

            if (next < 0)
                break;

            idx[next]++;
            for (int k = next + 1; k < m; k++)
                idx[k] = 0;
        }
        ////////////////////////////////////////////////////////////////////
    }
}

TPPACData *TLPPACAna::Processing(uint32_t tx1, uint32_t tx2,
                                 uint32_t ty1, uint32_t ty2,
                                 uint32_t ta, int lcr)
{
    float lcroffset[3] = { -132.5, 0, 132.5 };
    float x = (float(tx1) - float(tx2)) * factor[lcr] + offset[lcr] + lcroffset[lcr];
    float y = (float(ty1) - float(ty2)) * factor[3] + offset[3];

    new ((*outdata)[outdata->GetEntriesFast()]) TPPACData(tx1, tx2, ty1, ty2, ta, x, y);
    return static_cast<TPPACData *>(outdata->Last());
}

void TLPPACAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata);
    // tree->Branch(Form("%sLPPAC", name.c_str()), &flagSet, "flagSet/B");
}

void TLPPACAna::PrintParameters()
{
    std::cout << "TLPPACAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl;
    std::cout << " Channels: ";
    for (int i = 0; i < n; i++)
        std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " Offsets: ";
    for (int i = 0; i < 4; i++)
        std::cout << offset[i] << " ";
    std::cout << "\n";
    std::cout << " Factors: ";
    for (int i = 0; i < 4; i++)
        std::cout << factor[i] << " ";
    std::cout << "\n";
    std::cout << " TDC cuts: " << tdc_cut[0] << " " << tdc_cut[1];
    std::cout << "\n";
}

void TLPPACAna::PrintData()
{
    std::cout << "TLPPACAna: " << name << std::endl;
    const char *label[n] = {"XL_UP", "XL_DN", "XC_UP", "XC_DN", "XR_UP", "XR_DN", "Y_UP", "Y_DN", "Anode"};
    for (int j = 0; j < n; j++)
    {
        std::cout << " " << label[j] << ": ";
        for (int i = 0; i < int(data[j].size()); i++)
            std::cout << data[j][i] << " ";
        std::cout << "\n";
    }
}

void TLPPACAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TLPPACAna: " << name << std::endl;
    TPPACData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TPPACData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}