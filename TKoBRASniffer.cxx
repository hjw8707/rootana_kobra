#include "TKoBRASniffer.hxx"
#include "THistManager.hxx"
#include "TTreeManager.hxx"
#include "TCanvasManager.hxx"
#include <iostream>

ClassImp(TKoBRASniffer)

    TKoBRASniffer::TKoBRASniffer() : TRootSniffer("kobra_sniffer")
{
    RegisterCommand("/Clear", "this->HistClear();", "button;rootsys/icons/ed_delete.png");
    RegisterCommand("/Reload", "this->HistReload();", "button;rootsys/icons/refresh.png");    
    //Hide("/Clear");
    //Hide("/Reload");
}
TKoBRASniffer::~TKoBRASniffer() {}

Bool_t TKoBRASniffer::HistClear()
{
    std::cout << "Clear the hitograms" << std::endl;
    TTreeManager::GetInstance()->Reset();
    THistManager::GetInstance()->Reset();
    return true;
}

Bool_t TKoBRASniffer::HistReload()
{
    std::cout << "Reload the hitograms" << std::endl;
    TTreeManager::GetInstance()->Reset();
    THistManager::GetInstance()->Clear();
    THistManager::GetInstance()->AddHistFromFile("histlist.txt");
    TCanvasManager::GetInstance()->AddCanvasAndHistFromFile("canlist.txt");
    return true;
}