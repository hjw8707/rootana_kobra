#include "TScalerAna.hxx"
#include "TCollection.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

TScalerAna::TScalerAna(const char *_name, const char *_parfile)
{
    name = _name;
    flagSet = true;
    flagData = false;

    outdata = new TClonesArray("TScalerData", 10);
    SetParameters(_parfile);
}

TScalerAna::~TScalerAna()
{
    outdata->Clear("C");
    delete outdata;
};

void TScalerAna::Clear()
{
  data.clear();

  outdata->Clear("C");
}

void TScalerAna::SetParameters(const char *file)
{}

void TScalerAna::SetData(TGenericData *rdata)
{
    if (!flagSet)
        return;

    if (!rdata)
        return;

    if (rdata->GetSize() < 10) return;
    
    data.push_back(static_cast<TKoBRASCAData*>(rdata));

    flagData = true;
}

void TScalerAna::Analysis()
{
  if (!flagSet)
    return;
  if (!flagData)
    return;

  for (const auto &it: data) {
    Processing(it->GetEventTimeStamp(), it->GetAllPPACORCount(),
	       it->GetF2PlasticCount(), it->GetF3PlasticCount(),
	       it->GetHWTrigAfterLatchCount(), it->GetHWTrigBeforeLatchCount(),
	       it->GetRefPulse(), it->GetRefTime());
  }
  ////////////////////////////////////////////////////////////////////
}

TScalerData *TScalerAna::Processing(uint64_t _ts, uint32_t _ppac, uint32_t _f2pl, uint32_t _f3pl,
				    uint32_t _hwtriga, uint32_t _hwtrigb, uint32_t _refp, uint32_t _reft)
{
  new ((*outdata)[outdata->GetEntriesFast()]) TScalerData(_ts, _ppac, _f2pl, _f3pl, _hwtriga, _hwtrigb, _refp, _reft);
  return static_cast<TScalerData *>(outdata->Last());
}

void TScalerAna::SetTree()
{
    TTree *tree = TTreeManager::GetInstance()->GetTree();
    tree->Branch(Form("%s", name.c_str()), &outdata, 256000, 99);
    // tree->Branch(Form("%sppac", name.c_str()), &flagSet, "flagSet/B");
}

void TScalerAna::PrintParameters()
{
    std::cout << "TScalerAna: " << name << std::endl;
    std::cout << "\n";
}

void TScalerAna::PrintData()
{
  std::cout << "TScalerAna: " << name << std::endl;
  for (int j = 0; j < data.size(); j++)
    {
      std::cout << data[j] << " ";
    }
  std::cout << "\n";

}

void TScalerAna::PrintOutdata()
{
    if (outdata->IsEmpty())
        return;
    std::cout << "TScalerAna: " << name << std::endl;
    TScalerData *d;
    TIter next(outdata);
    int i = 0;
    while ((d = static_cast<TScalerData *>(next())))
    {
        std::cout << "Data " << i++ << ": ";
        d->Print();
    }
}
