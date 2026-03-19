#ifndef __CLING__
#include <fstream>

#include "TCanvas.h"
#include "TF1.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void brho_cor_mdis(const std::string& iso = "o20") {
  bool flagCut = true;
  std::map<int, std::vector<std::vector<int>>> runmap = KOBRA::mruns[iso];
  std::map<int, std::vector<std::pair<double, double>>> runmap_disp = KOBRA::mruns_disp[iso];
  std::map<int, std::vector<int>> runmap_hv = KOBRA::mruns_hv[iso];
  std::map<int, std::vector<double>> runmap_brho = KOBRA::mruns_brho[iso];

  KOBRA* ko;
  double aoq = (iso == "o20" ? 2.5 : 2.4);
  std::ofstream fout(Form("analysis/brho_cor_mdis_%s.txt", iso.c_str()));
  for (const auto& pair_key_rungroups : runmap) {
    int key = pair_key_rungroups.first;
    const std::vector<std::vector<int>>& run_groups = pair_key_rungroups.second;
    const std::vector<double>& brho_values = runmap_brho.at(key);

    // for (size_t i = 0; i < run_groups.size(); ++i) {
    for (size_t i = 0; i < 1; i++) {
      const std::vector<int>& current_run_group = run_groups[i];
      double current_brho = brho_values[i];
      ko = new KOBRA(KOBRA::Expt::Phys, current_run_group);
      ko->SetBrho(current_brho);
      ko->SetUsePla();
      ko->Draw(Form("aoqcorp>>haoqcor_r%d(400,2.35,2.55)", current_run_group.front()), iso.c_str());
      auto h1 = static_cast<TH1*>(gDirectory->Get(Form("haoqcor_r%d", current_run_group.front())));
      h1->Fit("gaus");
      TF1* f1 = h1->GetFunction("gaus");
      double mean = f1->GetParameter(1);
      fout << current_run_group.front() << " " << current_brho << " " << mean << " " << (mean / aoq) << " "
           << (current_brho * (aoq / mean)) << std::endl;
      delete ko;
    }
  }
  fout.close();
}

void brho_cor_mdis_o20() { brho_cor_mdis("o20"); }

void brho_cor_mdis_ne24() { brho_cor_mdis("ne24"); }