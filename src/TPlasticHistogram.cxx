#include "TPlasticHistogram.hxx"

#include "TPlasticData.hxx"
#include "TDirectory.h"
#include "TH2.h"

/// Reset the histograms for this canvas
TPlasticHistograms::TPlasticHistograms(const char *_name, TClonesArray *_data)
{
    name = _name;
    data = _data;
    SetSubTabName(_name);
    CreateHistograms();
}

void TPlasticHistograms::CreateHistograms()
{
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(name.c_str(), name.c_str());
    curDir->cd(name.c_str());

    std::cout << "Create Histos" << std::endl;
    char na[100];
    char title[100];

    const char *items1d[6] = {"tl", "tr", "al", "ar", "tdiff", "tave"};
    int nbins[6] = {1000, 1000, 1000, 1000, 1000, 1000};
    float low[6] = {0, 0, 0, 0, 0, 0};
    float upp[6] = {500000, 500000, 50000, 50000, 500000, 500000};
    for (int i = 0; i < 6; i++)
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

    sprintf(na, "%s_tlr", name.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    sprintf(title, "%s histogram for tlr", name.c_str());
    TH2D *tmp = new TH2D(na, title, 1000, 0, 500000, 1000, 0, 500000);
    tmp->SetXTitle("tl");
    tmp->SetYTitle("tr");
    push_back(tmp);

    sprintf(na, "%s_alr", name.c_str());
    old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    sprintf(title, "%s histogram for alr", name.c_str());
    tmp = new TH2D(na, title, 1000, 0, 50000, 1000, 0, 50000);
    tmp->SetXTitle("al");
    tmp->SetYTitle("ar");
    push_back(tmp);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TPlasticHistograms::UpdateHistograms(TDataContainer &)
{
    if (!data) return;
    for (int i = 0 ; i < data->GetEntriesFast() ; i++) {
        TPlasticData* d = static_cast<TPlasticData*>((*data)[i]);
        GetHistogram(0)->Fill(d->tl);
        GetHistogram(1)->Fill(d->tr);
        GetHistogram(2)->Fill(d->al);
        GetHistogram(3)->Fill(d->ar);
        GetHistogram(4)->Fill(d->tdiff);
        GetHistogram(5)->Fill(d->GetTAve());
        GetHistogram(6)->Fill(d->tl, d->tr);
        GetHistogram(7)->Fill(d->al, d->ar);
    }
}

/// Take actions at begin run
void TPlasticHistograms::BeginRun(int transition, int run, int time)
{
    CreateHistograms();
}

/// Take actions at end run
void TPlasticHistograms::EndRun(int transition, int run, int time)
{
}
