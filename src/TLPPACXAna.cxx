#include "TLPPACXAna.hxx"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "TCollection.h"

TLPPACXAna::TLPPACXAna(const char *_name, const char *_parfile) {
    name = _name;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TPPACData", 10, 1);
    SetParameters(_parfile);
}

TLPPACXAna::~TLPPACXAna() {
    outdata->Clear("C");
    delete outdata;
};

void TLPPACXAna::Clear() {
    for (int i = 0; i < n; i++) data[i].clear();

    outdata->Clear("C");
}

void TLPPACXAna::SetParameters(const char *file) {
    std::ifstream fin(file);
    if (not fin.is_open()) {
        std::cerr << "File cannot be opened." << std::endl;
        return;
    }
    int nline = 0;
    std::string line;
    while (std::getline(fin, line)) {
        // #으로 시작하는 주석을 제거
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);

        // 공백만 있는 줄은 무시
        if (line.empty() || line.find_first_not_of(' ') == std::string::npos) continue;

        uint32_t inum;
        float fnum;
        std::istringstream iss(line);
        switch (nline++) {
            case 0:
                while (iss >> inum) chs.push_back(inum);
                break;
            case 1:
                while (iss >> fnum) offset.push_back(fnum);
                break;
            case 2:
                while (iss >> fnum) factor.push_back(fnum);
                break;
            case 3:
                iss >> tdc_cut[0] >> tdc_cut[1];  // TDC Lower Limit, TDC Upper Limit
                break;
            case 4:
                iss >> tsum_cut[0] >> tsum_cut[1];  // X Lower Limit, X Upper Limit
                break;
            case 5:
                iss >> txsum_offset[0] >> txsum_offset[1] >> txsum_offset[2];  // [left, center, right]
                break;
            default:
                break;
        }
    }
    fin.close();
    flagSet = true;
}

// void TLPPACXAna::SetData(std::vector<KoBRATDCMeasurement> &_data)
void TLPPACXAna::SetData(TGenericData *rdata) {
    if (!flagSet) return;

    if (!rdata) return;

    std::vector<KoBRATDCMeasurement> _data = static_cast<TKoBRATDCData *>(rdata)->GetMeasurements();

    for (auto it = _data.begin(); it != _data.end(); it++) {
        for (int i = 0; i < n; i++) {
            if (chs[i] == it->GetChannel() && it->GetMeasurement() > tdc_cut[0] && it->GetMeasurement() < tdc_cut[1])
                data[i].push_back(it->GetMeasurement());
        }
    }

    flagData = true;
    //    for (int i = 6; i < n; i++) // at least yup, ydn, anode
    if (data[6].empty())  // only the case that anode is empty
        flagData = false;
    if ((data[0].empty() || data[1].empty()) && (data[2].empty() || data[3].empty()) &&
        (data[4].empty() || data[5].empty()))
        flagData = false;
}

void TLPPACXAna::Analysis() {
    if (!flagSet) return;
    if (!flagData) return;

    const int m = 5;
    uint32_t *idx = new uint32_t[m];
    std::vector<uint32_t> arr[m];
    arr[2] = data[6];
    for (int i = 0; i < 3; i++) {
        if (data[2 * i].empty() || data[2 * i + 1].empty()) continue;

        arr[0] = data[2 * i];
        arr[1] = data[2 * i + 1];
        for (int j = 0; j < m; j++) idx[j] = 0;

        // all combinations
        while (1) {
            Processing(arr[0][idx[0]], arr[1][idx[1]], arr[2][idx[2]], i);
            int next = m - 1;
            while (next >= 0 && idx[next] + 1 >= data[next].size()) next--;

            if (next < 0) break;

            idx[next]++;
            for (int k = next + 1; k < m; k++) idx[k] = 0;
        }
        ////////////////////////////////////////////////////////////////////
    }
}

TPPACData *TLPPACXAna::Processing(uint32_t tx1, uint32_t tx2, uint32_t ta, int lcr) {
    if (static_cast<float>(tx1 + tx2 - 2 * ta) + txsum_offset[lcr] < tsum_cut[0] ||
        static_cast<float>(tx1 + tx2 - 2 * ta) + txsum_offset[lcr] > tsum_cut[1])
        return nullptr;  // TXSum Cut

    float lcroffset[3] = {-132.5, 0, 132.5};
    float x = (float(tx1) - float(tx2)) * factor[lcr] + offset[lcr] + lcroffset[lcr];
    float txsum = static_cast<float>(tx1 + tx2 - 2 * ta) + txsum_offset[lcr];

    new ((*outdata)[outdata->GetEntriesFast()]) TPPACData(tx1, tx2, 0, 0, ta, x, 0, txsum, 0);
    return static_cast<TPPACData *>(outdata->Last());
}

void TLPPACXAna::SetTree() {
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata);
    // tree->Branch(Form("%sLPPAC", name.c_str()), &flagSet, "flagSet/B");
}

void TLPPACXAna::PrintParameters() {
    std::cout << "TLPPACXAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl;
    std::cout << " Channels: ";
    for (int i = 0; i < n; i++) std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " Offsets: ";
    for (int i = 0; i < 4; i++) std::cout << offset[i] << " ";
    std::cout << "\n";
    std::cout << " Factors: ";
    for (int i = 0; i < 4; i++) std::cout << factor[i] << " ";
    std::cout << "\n";
    std::cout << " TDC cuts: " << tdc_cut[0] << " " << tdc_cut[1] << "\n";
    std::cout << " TXSum cuts: " << tsum_cut[0] << " " << tsum_cut[1] << "\n";
    std::cout << " TXSum offsets: " << txsum_offset[0] << " " << txsum_offset[1] << " " << txsum_offset[2] << "\n";
}

void TLPPACXAna::PrintData() {
    std::cout << "TLPPACAna: " << name << std::endl;
    const char *label[n] = {"XL_UP", "XL_DN", "XC_UP", "XC_DN", "XR_UP", "XR_DN", "Anode"};
    for (int j = 0; j < n; j++) {
        std::cout << " " << label[j] << ": ";
        for (int i = 0; i < int(data[j].size()); i++) std::cout << data[j][i] << " ";
        std::cout << "\n";
    }
}

void TLPPACXAna::PrintOutdata() {
    if (outdata->IsEmpty()) return;
    std::cout << "TLPPACXAna: " << name << std::endl;
    TPPACData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TPPACData *>(next()))) {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}
