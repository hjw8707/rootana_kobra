#include "TTrackHistogram.hxx"

#include "TPPACData.hxx"
#include "TDirectory.h"
#include "TH2.h"

#include <algorithm>

/// Reset the histograms for this canvas
TTrackHistograms::TTrackHistograms(const char *_name, TClonesArray *_ppacup, TClonesArray *_ppacdn, float _dist)
{
    name = _name;
    ppacup = _ppacup;
    ppacdn = _ppacdn;
    dist = _dist;
    SetSubTabName(_name);
    CreateHistograms();
}

void TTrackHistograms::CreateHistograms()
{
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(name.c_str(), name.c_str());
    curDir->cd(name.c_str());

    std::cout << "Create Histos for " << name << std::endl;
    char na[100];
    char title[100];

    const int n = 2;
    const char *items1d[n] = {"a", "b"};
    int nbins[n] = {1000, 1000};
    float low[n] = {-100, -100};
    float upp[n] = {100, 100};
    for (int i = 0; i < n; i++)
    {
        sprintf(na, "%s_%s", name.c_str(), items1d[i]);
        // Delete old histograms, if we already have them
        TH1D *old = (TH1D *)gDirectory->Get(na);
        if (old)
            delete old;

        // Create new histograms
        sprintf(title, "%s histogram for %s", name.c_str(), items1d[i]);

        TH1D *tmp = new TH1D(na, title, nbins[i], low[i], upp[i]);
        tmp->SetXTitle(items1d[i]);
        tmp->SetYTitle("Number of Entries");
        push_back(tmp);
    }

    sprintf(na, "%s_ab", name.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    sprintf(title, "%s B vs A", name.c_str());
    TH2D *tmp = new TH2D(na, title, 100, -100, 100, 100, -100, 100);
    tmp->SetXTitle("A [mrad]");
    tmp->SetYTitle("B [mrad]");
    push_back(tmp);

    /////////////////////////////////////////////////////////////////////
    // eff
    sprintf(na, "%s_%s", name.c_str(), "effup");
    // Delete old histograms, if we already have them
    TH1D* old1 = (TH1D *)gDirectory->Get(na);
    if (old1)
        delete old1;

    // Create new histograms
    sprintf(title, "%s histogram for %s", name.c_str(), "effup");

    TH1D* tmp1 = new TH1D(na, title, 2, 0, 2);
    tmp1->SetXTitle("Detected or Not");
    tmp1->SetYTitle("Number of Entries");
    push_back(tmp1);

    sprintf(na, "%s_%s", name.c_str(), "effdn");
    // Delete old histograms, if we already have them
    old1 = (TH1D *)gDirectory->Get(na);
    if (old1)
        delete old1;

    // Create new histograms
    sprintf(title, "%s histogram for %s", name.c_str(), "effdn");

    tmp1 = new TH1D(na, title, 2, 0, 2);
    tmp1->SetXTitle("Detected or Not");
    tmp1->SetYTitle("Number of Entries");
    push_back(tmp1);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TTrackHistograms::UpdateHistograms(TDataContainer &)
{
    for (int i = 0; i < ppacup->GetEntriesFast(); i++)
    {
        TPPACData *pup = static_cast<TPPACData *>((*ppacup)[i]);
        for (int j = 0; j < ppacdn->GetEntriesFast(); j++)
        {
            TPPACData *pdn = static_cast<TPPACData *>((*ppacdn)[j]);
            float a = (pdn->x - pup->x) / dist;
            float b = (pdn->y - pup->y) / dist;
            GetHistogram(0)->Fill(a);
            GetHistogram(1)->Fill(b);
            GetHistogram(2)->Fill(a, b);
        }
    }
    if (ppacdn->GetEntriesFast() > 0)
        GetHistogram(3)->Fill((ppacup->GetEntriesFast() > 0));
    if (ppacup->GetEntriesFast() > 0)
        GetHistogram(4)->Fill((ppacdn->GetEntriesFast() > 0));
}

/// Take actions at begin run
void TTrackHistograms::BeginRun(int transition, int run, int time)
{
    CreateHistograms();
}

/// Take actions at end run
void TTrackHistograms::EndRun(int transition, int run, int time)
{
}
