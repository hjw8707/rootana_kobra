#ifndef __CLING__
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TCutG.h"
#include "kobra.hxx"
#endif

void ppac_timing() {
    bool flagCut = true;
    const std::string iso = "o20";
    std::map<int, std::vector<std::vector<int>>> runmap = KOBRA::mruns[iso];
    std::map<int, std::vector<std::pair<double, double>>> runmap_disp = KOBRA::mruns_disp[iso];
    std::map<int, std::vector<int>> runmap_hv = KOBRA::mruns_hv[iso];
    std::map<int, std::vector<double>> runmap_brho = KOBRA::mruns_brho[iso];

    KOBRA* ko;
    for (const auto& pair_key_rungroups : runmap) {
        int key = pair_key_rungroups.first;
        const std::vector<std::vector<int>>& run_groups = pair_key_rungroups.second;
        const std::vector<double>& brho_values = runmap_brho.at(key);

        for (size_t i = 0; i < run_groups.size(); ++i) {
            const std::vector<int>& current_run_group = run_groups[i];
            double current_brho = brho_values[i];
            ko = new KOBRA(KOBRA::Expt::Phys, current_run_group);
            ko->DrawPPACTimings(false);
            gPad->GetCanvas()->Print(
                Form("figs/ppac_timing_%s_%c%d_%zu.pdf", iso.c_str(), key > 0 ? 'p' : 'n', std::abs(key), i));
            delete ko;
        }
    }
}