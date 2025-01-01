#include "TPPACHistogram.hxx"

#include "TPPACData.hxx"
#include "TDirectory.h"
#include "TH2.h"

/// Reset the histograms for this canvas
TPPACHistograms::TPPACHistograms(const char *_name, TClonesArray *_data)
{
    name = _name;
    data = _data;
    SetSubTabName(_name);
    CreateHistograms();
}

void TPPACHistograms::CreateHistograms()
{
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(name.c_str(), name.c_str());
    curDir->cd(name.c_str());

    std::cout << "Create Histos" << std::endl;
    char na[100];
    char title[100];

    const char *items1d[9] = {"x1", "x2", "y1", "y2", "a", "txsum", "tysum", "x", "y"};
    int nbins[9] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    float low[9] = {0, 0, 0, 0, 0, 0, 0, -100, -100};
    float upp[9] = {50000, 50000, 50000, 50000, 50000, 50000, 50000, 100, 100};
    for (int i = 0; i < 9; i++)
    {
      snprintf(na, sizeof(na), "%s_%s", name.c_str(), items1d[i]);
        // Delete old histograms, if we already have them
        TH1D *old = (TH1D *)gDirectory->Get(na);
        if (old)
            delete old;

        // Create new histograms
        snprintf(title, sizeof(title), "%s histogram for %s", name.c_str(), items1d[i]);

        TH1D *tmp = new TH1D(na, title, nbins[i], low[i], upp[i]);
        tmp->SetXTitle(items1d[i]);
        tmp->SetYTitle("Number of Entries");
        push_back(tmp);
    }

    snprintf(na, sizeof(na), "%s_xy", name.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    snprintf(title, sizeof(title), "%s histogram for xy", name.c_str());
    TH2D *tmp = new TH2D(na, title, 1000, -100, 100, 1000, -100, 100);
    tmp->SetXTitle("x");
    tmp->SetYTitle("y");
    push_back(tmp);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TPPACHistograms::UpdateHistograms(TDataContainer &)
{
    if (!data) return;
    for (int i = 0 ; i < data->GetEntriesFast() ; i++) {
        TPPACData* d = static_cast<TPPACData*>((*data)[i]);
        GetHistogram(0)->Fill(d->tx1);
        GetHistogram(1)->Fill(d->tx2);
        GetHistogram(2)->Fill(d->ty1);
        GetHistogram(3)->Fill(d->ty2);
        GetHistogram(4)->Fill(d->ta);
        GetHistogram(5)->Fill(d->GetTXSum());
        GetHistogram(6)->Fill(d->GetTYSum());
        GetHistogram(7)->Fill(d->x);
        GetHistogram(8)->Fill(d->y);
        GetHistogram(9)->Fill(d->x, d->y);
    }
}

/// Take actions at begin run
void TPPACHistograms::BeginRun(int transition, int run, int time)
{
    CreateHistograms();
}

/// Take actions at end run
void TPPACHistograms::EndRun(int transition, int run, int time)
{
}
