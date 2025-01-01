#include "TPIDHistogram.hxx"

#include "TPlasticData.hxx"
#include "TSSDData.hxx"
#include "TDirectory.h"
#include "TH2.h"

#include <algorithm>

/// Reset the histograms for this canvas
TPIDHistograms::TPIDHistograms(const char *_name, TClonesArray *_f2pladata, TClonesArray *_f3pladata, TClonesArray *_f2ssddata, TClonesArray *_f3ssddata)
{
    name = _name;
    f2pladata = _f2pladata;
    f3pladata = _f3pladata;
    f2ssddata = _f2ssddata;
    f3ssddata = _f3ssddata;
    SetSubTabName(_name);
    CreateHistograms();
}

void TPIDHistograms::CreateHistograms()
{
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(name.c_str(), name.c_str());
    curDir->cd(name.c_str());

    std::cout << "Create Histos" << std::endl;
    char na[100];
    char title[100];

    const int n = 1;
    const char *items1d[n] = {"tof"};
    int nbins[n] = {1000};
    float low[n] = {0};
    float upp[n] = {500};
    for (int i = 0; i < n; i++)
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

    snprintf(na, sizeof(na), "%s_f2ssd_vs_tof", name.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    snprintf(title, sizeof(title), "F2SSD vs TOF(F2-F3)");
    TH2D *tmp = new TH2D(na, title, 1000, 0, 500, 1000, 0, 100);
    tmp->SetXTitle("TOF(F2-F3)");
    tmp->SetYTitle("F2SSD");
    push_back(tmp);

    snprintf(na, sizeof(na), "%s_f3ssd_vs_tof", name.c_str());
    old = (TH2D *)gDirectory->Get(na);
    if (old)
        delete old;
    snprintf(title, sizeof(title), "F3SSD vs TOF(F2-F3)");
    tmp = new TH2D(na, title, 1000, 0, 500, 1000, 0, 100);
    tmp->SetXTitle("TOF(F2-F3)");
    tmp->SetYTitle("F3SSD");
    push_back(tmp);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TPIDHistograms::UpdateHistograms(TDataContainer &)
{
    std::vector<float> tofs;
    for (int i = 0; i < f2pladata->GetEntriesFast(); i++)
    {
        TPlasticData *d_f2 = static_cast<TPlasticData *>((*f2pladata)[i]);
        for (int j = 0; j < f3pladata->GetEntriesFast(); j++)
        {
            TPlasticData *d_f3 = static_cast<TPlasticData *>((*f3pladata)[j]);
            tofs.push_back(d_f3->GetTAve() - d_f2->GetTAve());
        }
    }
    if (tofs.empty())
        return;

    for (const auto &tof : tofs)
    {
        GetHistogram(0)->Fill(tof);
        for (int i = 0; i < f2ssddata->GetEntriesFast(); i++)
        {
            TSSDData *d_f2ssd = static_cast<TSSDData *>((*f2ssddata)[i]);
            GetHistogram(1)->Fill(tof, d_f2ssd->acal);
        }
        for (int i = 0; i < f3ssddata->GetEntriesFast(); i++)
        {
            TSSDData *d_f3ssd = static_cast<TSSDData *>((*f3ssddata)[i]);
            GetHistogram(2)->Fill(tof, d_f3ssd->acal);
        }
    }
}

/// Take actions at begin run
void TPIDHistograms::BeginRun(int transition, int run, int time)
{
    CreateHistograms();
}

/// Take actions at end run
void TPIDHistograms::EndRun(int transition, int run, int time)
{
}
