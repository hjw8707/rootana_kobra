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
    RegisterCommand("/Fit", "this->HistFit(\"%arg1%\");", "button;rootsys/icons/bld_edit.png");
    SetItemField("/Reload", "_hreload", "true");

    runnum = new TObjString("RunNum");
    RegisterObject("/", runnum);

    runstat = new TObjString("RunStatus");
    RegisterObject("/", runstat);
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
    TTreeManager::GetInstance()->LoadAlias("aliaslist.txt");
    return true;
}

Bool_t TKoBRASniffer::HistFit(const char *name)
{

    TH1 *h1 = THistManager::GetInstance()->Get1DHist(name);
    if (h1)
        h1->Fit("gaus");
    return true;
}

void TKoBRASniffer::SetRunNumber(int run)
{
    runnum->SetString(Form("Run = %05d", run));
}

void TKoBRASniffer::SetRunStatus(bool running)
{
    if (running)
        runstat->SetString("Status = Running");
    else
        runstat->SetString("Status = Idle");
}