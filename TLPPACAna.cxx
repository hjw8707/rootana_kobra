#include "TLPPACAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <string>

TLPPACAna::TLPPACAna(const char *_name, const char *_parfile)
{
    name = _name;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPPACData");
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

    fin >> chs[0] >> chs[1] >> chs[2] >> chs[3] >> chs[4] >> chs[5] >> chs[6] >> chs[7] >> chs[8];
    fin >> offset[0] >> offset[1] >> offset[2] >> offset[3];
    fin >> factor[0] >> factor[1] >> factor[2] >> factor[3];
    fin >> tdc_cut[0] >> tdc_cut[1];
    fin.close();

    flagSet = true;
}

void TLPPACAna::SetData(std::vector<KoBRATDCMeasurement> &_data)
{
    if (!flagSet)
        return;

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
    if ( (data[0].empty()||data[1].empty()) &&
        (data[2].empty()||data[3].empty()) &&
        (data[4].empty()||data[5].empty()))
        flagData = false;   
}

void TLPPACAna::Analysis()
{
    if (!flagSet)  return;
    if (!flagData) return;

    const int m = 5;
    uint32_t *idx = new uint32_t[m];
    std::vector<uint32_t> arr[m];
    arr[2] = data[6];
    arr[3] = data[7];
    arr[4] = data[8];
    for (int i = 0 ; i < 3 ; i++) {
        if (data[2*i].empty()||data[2*i+1].empty()) continue;

        arr[0] = data[2*i];
        arr[1] = data[2*i+1];       
        for (int j = 0; j < m; j++) idx[j] = 0;

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
    float x = (float(tx1) - float(tx2)) * factor[lcr] + offset[lcr];
    float y = (float(ty1) - float(ty2)) * factor[3] + offset[3];

    new ((*outdata)[outdata->GetEntriesFast()]) TPPACData(tx1, tx2, ty1, ty2, ta, x, y);
    return static_cast<TPPACData *>(outdata->Last());
}

void TLPPACAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%sppac", name.c_str()), &outdata);
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