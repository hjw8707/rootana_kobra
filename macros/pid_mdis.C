#ifndef __CLING__
#include <fstream>

#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void pid_mdis() {
    bool flagCut = true;
    const std::string iso = "ne24";
    std::map<int, std::vector<std::vector<int>>> runmap = KOBRA::mruns[iso];
    std::map<int, std::vector<std::pair<double, double>>> runmap_disp = KOBRA::mruns_disp[iso];
    std::map<int, std::vector<int>> runmap_hv = KOBRA::mruns_hv[iso];
    std::map<int, std::vector<double>> runmap_brho = KOBRA::mruns_brho[iso];

    KOBRA* ko;
    TH2* pid_sum = nullptr;

    std::ofstream fout("pid_mdis_counts.txt");
    for (const auto& pair_key_rungroups : runmap) {
        int key = pair_key_rungroups.first;
        const std::vector<std::vector<int>>& run_groups = pair_key_rungroups.second;
        const std::vector<double>& brho_values = runmap_brho.at(key);

        for (size_t i = 0; i < run_groups.size(); ++i) {
            const std::vector<int>& current_run_group = run_groups[i];
            double current_brho = brho_values[i];
            ko = new KOBRA(KOBRA::Expt::Phys, current_run_group);
            ko->SetBrho(current_brho);
            ko->SetUsePla();
            TH2* h2 = ko->DrawPID(0);
            if (pid_sum == nullptr) {
                pid_sum = static_cast<TH2*>(h2->Clone("pid_sum"));
                pid_sum->SetDirectory(nullptr);
            } else {
                pid_sum->Add(h2);
            }
            gPad->GetCanvas()->Print(
                Form("figs/pid_mdis_%s_%c%d_%zu.png", iso.c_str(), key > 0 ? 'p' : 'n', std::abs(key), i));
            ko->CountIsotopesRunByRun(fout, false, false);
            gPad->GetCanvas()->Update();
            getchar();
            delete gPad->GetCanvas();
            delete ko;
        }
    }
    pid_sum->Draw("colz");
    // KOBRA ko2(KOBRA::Expt::Phys, KOBRA::mruns["o20"][0][0], KOBRA::mruns_brho["o20"][0][0]);
    // ko2.DrawAllCuts(true);
    fout.close();
}