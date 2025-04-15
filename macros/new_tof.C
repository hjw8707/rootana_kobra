#ifndef __CLING__
#include "TCanvas.h"
#include "TChain.h"
#include "TEventList.h"
#include "TF1.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void new_tof() {
    const int n = 5;
    int a[] = {18, 19, 20, 21, 22};
    double brho[] = {1.2959, 1.3198, 1.3734, 1.3879, 1.4600};

    std::vector<int> expt[] = {KOBRA::o18, KOBRA::o19, KOBRA::o20, KOBRA::o21, KOBRA::o22};

    std::vector<double> old_brho;
    std::vector<double> new_brho;
    std::vector<double> correction;

    for (int i = 0; i < n; i++) {
        KOBRA *ko = new KOBRA(KOBRA::Expt::Phys, expt[i]);
        old_brho.push_back(ko->GetBrho());
        ko->SetBrho(brho[i]);
        ko->tree->Draw(Form(">>e_o%d", a[i]), Form("o%d", a[i]));
        auto e = static_cast<TEventList *>(gDirectory->Get(Form("e_o%d", a[i])));
        ko->tree->SetEventList(e);

        ko->SetPathLength(12507.2);
        ko->tree->Draw("aoq>>h_aoq(400,2,3)");
        auto h_aoq = static_cast<TH1 *>(gDirectory->Get("h_aoq"));
        h_aoq->Fit("gaus");
        auto fit_result = h_aoq->GetFunction("gaus");
        auto mean = fit_result->GetParameter(1);
        auto sigma = fit_result->GetParameter(2);
        auto width = 2.35 * sigma;
        std::cout << "Mean: " << mean << ", Sigma: " << sigma << ", Width: " << width << std::endl;
        std::cout << "A/Q: " << (a[i] / 8.0) << std::endl;
        double aoq = a[i] / 8.0;
        std::cout << "New Brho: " << brho[i] * aoq / mean << std::endl;
        std::cout << "Correction factor: " << (brho[i] * aoq / mean / old_brho[i] - 1.0) * 100 << "%" << std::endl;

        new_brho.push_back(brho[i] * aoq / mean);
        correction.push_back((brho[i] * aoq / mean / old_brho[i] - 1.0) * 100);

        delete ko;
    }

    for (int i = 0; i < n; i++) {
        std::cout << "Experiment " << a[i] << ": " << old_brho[i] << " -> " << new_brho[i] << " (" << correction[i]
                  << "%)" << std::endl;
    }
}
