#include "TSSDHistogram.h"

#include "TSSDData.hxx"
#include "TDirectory.h"
#include "TH2.h"

#include <algorithm>

/// Reset the histograms for this canvas
TSSDHistograms::TSSDHistograms(const char *_name, TClonesArray *_data)
{
    name = _name;
    data = _data;
    SetSubTabName(_name);
    CreateHistograms();
}

void TSSDHistograms::CreateHistograms()
{
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(name.c_str(), name.c_str());
    curDir->cd(name.c_str());

    std::cout << "Create Histos" << std::endl;
    char na[100];
    char title[100];

    const char *items1d[4] = {"ch", "adc", "acal", "acal(max)"};
    int nbins[4] = {32, 10000, 100, 1000};
    float low[4] = {0, 0, 0, 0};
    float upp[4] = {32, 10000, 10000, 10000};
    for (int i = 0; i < 4; i++)
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

    sprintf(na, "%s_chadc", name.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    sprintf(title, "%s histogram for chadc", name.c_str());
    TH2D *tmp = new TH2D(na, title, 32, 0, 32, 1000, 0, 10000);
    tmp->SetXTitle("ch");
    tmp->SetYTitle("adc");
    push_back(tmp);

    sprintf(na, "%s_chacal", name.c_str());
    old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    sprintf(title, "%s histogram for chacal", name.c_str());
    tmp = new TH2D(na, title, 32, 0, 32, 1000, 0, 100);
    tmp->SetXTitle("ch");
    tmp->SetYTitle("acal");
    push_back(tmp);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TSSDHistograms::UpdateHistograms(TDataContainer &)
{
    if (!data) return;
    std::vector<float> acals;
    for (int i = 0 ; i < data->GetEntriesFast() ; i++) {
        TSSDData* d = static_cast<TSSDData*>((*data)[i]);
        GetHistogram(0)->Fill(d->ch);
        GetHistogram(1)->Fill(d->adc);
        GetHistogram(2)->Fill(d->acal);
        acals.push_back(d->acal);
        GetHistogram(4)->Fill(d->ch, d->adc);
        GetHistogram(5)->Fill(d->ch, d->acal);
    }
    if (!acals.empty())
        GetHistogram(3)->Fill(*std::max_element(acals.begin(), acals.end()));
}

/// Take actions at begin run
void TSSDHistograms::BeginRun(int transition, int run, int time)
{
    CreateHistograms();
}

/// Take actions at end run
void TSSDHistograms::EndRun(int transition, int run, int time)
{
}
