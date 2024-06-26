#include "TSSDAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <string>

TSSDAna::TSSDAna(const char* _name, const char* _parfile) {
    name = _name;
    flagSet = false;
    flagData = false;

    outdata = new TClonesArray("TSSDData", 10);
    SetParameters(_parfile);
}

TSSDAna::~TSSDAna(){
    outdata->Clear("C");
    delete outdata;
};

void TSSDAna::Clear() {
    for (int i = 0 ; i < n ; i++)
        data[i].clear();

    outdata->Clear("C");
}

void TSSDAna::SetParameters(const char* file) {
    std::ifstream fin(file);
    if (not fin.is_open()) {
        std::cerr << "File cannot be opened." << std::endl;
        return;}

    fin >> chs[0] >> chs[1] >> chs[2] >> chs[3] >> chs[4] >> chs[5] >> chs[6] >> chs[7] >> chs[8] >> chs[9] >> chs[10] >> chs[11] >> chs[12] >> chs[13] >> chs[14] >> chs[15];
    fin >> ped[0] >> ped[1] >> ped[2] >> ped[3] >> ped[4] >> ped[5] >> ped[6] >> ped[7] >> ped[8] 
    >> ped[9] >> ped[10] >> ped[11] >> ped[12] >> ped[13] >> ped[14] >> ped[15];    
    fin >> offset[0] >> offset[1] >> offset[2] >> offset[3] >> offset[4] >> offset[5] >> offset[6] >> offset[7] >> offset[8] 
    >> offset[9] >> offset[10] >> offset[11] >> offset[12] >> offset[13] >> offset[14] >> offset[15];
    fin >> factor[0] >> factor[1] >> factor[2] >> factor[3] >> factor[4] >> factor[5] >> factor[6] >> factor[7] >> factor[8] 
    >> factor[9] >> factor[10] >> factor[11] >> factor[12] >> factor[13] >> factor[14] >> factor[15];        
    fin >> adc_cut[0] >> adc_cut[1];
    fin.close();
    
    flagSet = true;
}

void TSSDAna::SetData(std::vector<KoBRAADCMeasurement>& _data) {
    if (!flagSet) return;

    for (auto it = _data.begin() ; it != _data.end() ; it++) {
        for (int i = 0 ; i < n ; i++) {
            if (chs[i] == it->GetChannel() &&
                it->GetMeasurement() > adc_cut[0] &&
                it->GetMeasurement() < adc_cut[1])
                data[i].push_back(it->GetMeasurement());
        }
    }

    flagData = false;
    for (int i = 0 ; i < n ; i++) 
        if (!data[i].empty())
            flagData = true;
}

void TSSDAna::Analysis() {
    if (!flagSet) return;
    if (!flagData) return;

    ////////////////////////////////////////////////////////////////////
    // put each data
    for (int i = 0 ; i < n ; i++) { // for each channel
        for (auto it = data[i].begin() ; it != data[i].end() ; it++) {
            Processing(i, *it);            
        }} 
}

TSSDData* TSSDAna::Processing(int ch, uint32_t adc){
    new((*outdata)[outdata->GetEntriesFast()]) TSSDData(ch, adc, (float(adc) - ped[ch])*factor[ch]+offset[ch]);
    return static_cast<TSSDData*>(outdata->Last());
}

void TSSDAna::SetTree() {
    TTree* tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%sssd", name.c_str()), &outdata);
    //tree->Branch(Form("%sppac", name.c_str()), &flagSet, "flagSet/B");
}

void TSSDAna::PrintParameters() {
    std::cout << "TSSDAna: " << name << std::endl;
    std::cout << " Parameters" << std::endl; 
    std::cout << " Channels: ";
    for (int i = 0 ; i < n; i++)
        std::cout << chs[i] << " ";
    std::cout << "\n";
    std::cout << " Pedestal: ";
    for (int i = 0 ; i < n ; i++)
        std::cout << ped[i] << " ";
    std::cout << "\n";    
    std::cout << " Offsets: ";
    for (int i = 0 ; i < n ; i++)
        std::cout << offset[i] << " ";
    std::cout << "\n";
    std::cout << " Factors: ";
    for (int i = 0 ; i < n ; i++)
        std::cout << factor[i] << " ";
    std::cout << "\n";
    std::cout << " ADC cuts: " << adc_cut[0] << " " << adc_cut[1];
    std::cout << "\n";
}

void TSSDAna::PrintData() {
    std::cout << "TSSDAna: " << name << std::endl;
    for (int j = 0 ; j < n ; j++) {
        std::cout << " CH " << j << ": ";
        for (int i = 0 ; i < int(data[j].size()) ; i++)
            std::cout << data[j][i] << " ";
        std::cout << "\n"; }    
}

void TSSDAna::PrintOutdata() {
    if (outdata->IsEmpty()) return;
    std::cout << "TSSDAna: " << name << std::endl;
    TSSDData* d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TSSDData*>(next()))) {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}