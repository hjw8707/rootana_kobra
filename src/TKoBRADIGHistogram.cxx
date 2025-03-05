#include "TKoBRADIGHistogram.hxx"

#include "TDirectory.h"
#include "TH2.h"
#include "TKoBRADIGData.hxx"

const int Nchannels = 32;

/// Reset the histograms for this canvas
TKoBRADIGHistograms::TKoBRADIGHistograms(const char *_bankname) {
    bankname = _bankname;
    SetSubTabName(_bankname);
    CreateHistograms();
}

void TKoBRADIGHistograms::CreateHistograms() {
    // Otherwise make histograms
    clear();

    TDirectory *curDir = gDirectory;
    curDir->mkdir(bankname.c_str(), bankname.c_str());
    curDir->cd(bankname.c_str());

    std::cout << "Create Histos" << std::endl;
    for (int i = 0; i < Nchannels; i++) {  // loop over channels

        char name[100];
        char title[100];
        snprintf(name, sizeof(name), "%s_%i_t", bankname.c_str(), i);

        // Delete old histograms, if we already have them
        TH1D *old = (TH1D *)gDirectory->Get(name);
        if (old) delete old;

        // Create new histograms
        snprintf(title, sizeof(title), "%s Time histogram for channel=%i", bankname.c_str(), i);

        TH1D *tmp = new TH1D(name, title, 5000, 0, 50000000);
        tmp->SetXTitle("Time value");
        tmp->SetYTitle("Number of Entries");
        push_back(tmp);

        snprintf(name, sizeof(name), "%s_%i_a", bankname.c_str(), i);

        // Delete old histograms, if we already have them
        old = (TH1D *)gDirectory->Get(name);
        if (old) delete old;

        // Create new histograms
        snprintf(title, sizeof(title), "%s ADC histogram for channel=%i", bankname.c_str(), i);

        tmp = new TH1D(name, title, 5000, 0, 50000);
        tmp->SetXTitle("ADC value");
        tmp->SetYTitle("Number of Entries");
        push_back(tmp);
    }

    char name[100];
    char title[100];
    snprintf(name, sizeof(name), "%s_tall", bankname.c_str());
    TH2D *old = (TH2D *)gDirectory->Get(name);
    if (old) delete old;
    snprintf(title, sizeof(title), "%s time histogram for all", bankname.c_str());
    TH2D *tmp = new TH2D(name, title, Nchannels, 0, Nchannels, 5000, 0, 50000000);
    tmp->SetXTitle("Channel");
    tmp->SetYTitle("Time value");
    push_back(tmp);

    snprintf(name, sizeof(name), "%s_aall", bankname.c_str());
    old = (TH2D *)gDirectory->Get(name);
    if (old) delete old;
    snprintf(title, sizeof(title), "%s ADC histogram for all", bankname.c_str());
    tmp = new TH2D(name, title, Nchannels, 0, Nchannels, 5000, 0, 50000);
    tmp->SetXTitle("Channel");
    tmp->SetYTitle("ADC value");
    push_back(tmp);

    curDir->cd();
}

/// Update the histograms for this canvas.
void TKoBRADIGHistograms::UpdateHistograms(TDataContainer &dataContainer) {
    TKoBRADIGData *data = dataContainer.GetEventData<TKoBRADIGData>(bankname.c_str());
    if (!data) return;

    /// Get the Vector of ADC Measurements.
    std::vector<KoBRADIGMeasurement> measurements = data->GetMeasurements();
    for (unsigned int i = 0; i < measurements.size(); i++) {  // loop over measurements
        int chan = measurements[i].GetChannel();
        GetHistogram(chan * 2)->Fill(measurements[i].GetTime());
        GetHistogram(chan * 2 + 1)->Fill(measurements[i].GetEnergy());
        GetHistogram(Nchannels * 2)->Fill(chan, measurements[i].GetTime());
        GetHistogram(Nchannels * 2 + 1)->Fill(chan, measurements[i].GetEnergy());
    }
}

/// Take actions at begin run
void TKoBRADIGHistograms::BeginRun(int transition, int run, int time) { CreateHistograms(); }

/// Take actions at end run
void TKoBRADIGHistograms::EndRun(int transition, int run, int time) {}
