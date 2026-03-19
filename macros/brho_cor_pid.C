#ifndef __CLING__
#include <fstream>

#include "TCanvas.h"
#include "TF1.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void brho_cor_pid(bool flag_o = true) {
  int n_mass = flag_o ? 5 : 3;
  int o_mass[] = {18, 19, 20, 21, 22};
  int ne_mass[] = {24, 25, 26};
  std::vector<TH2 *> h_pid_list;

  std::ofstream fout(Form("analysis/brho_cor_pid_%s.txt", flag_o ? "o" : "ne"));
  for (int i = 0; i < n_mass; i++) {
    std::string mass_str = flag_o ? "o" + std::to_string(o_mass[i]) : "ne" + std::to_string(ne_mass[i]);
    // double aoq = (flag_o ? o_mass[i] / 8.0 : ne_mass[i] / 10.0);
    double aoq = 24 / 10.0;
    KOBRA ko_o(KOBRA::Expt::Phys, KOBRA::csruns[mass_str], KOBRA::brhoValue[mass_str]);

    // ko_o.Draw(Form("aoqcor>>haoqcor_r%d(200,%f,%f)", ko_o.runNs.front(), aoq - 0.05, aoq + 0.05), mass_str.c_str());
    ko_o.Draw(Form("aoqcorp>>haoqcor_r%d(200,%f,%f)", ko_o.runNs.front(), aoq - 0.05, aoq + 0.05), "ne24");
    auto h1 = static_cast<TH1 *>(gDirectory->Get(Form("haoqcor_r%d", ko_o.runNs.front())));
    h1->Fit("gaus");
    TF1 *f1 = h1->GetFunction("gaus");
    double mean = f1->GetParameter(1);
    fout << ko_o.runNs.front() << " " << ko_o.GetBrho() << " " << mean << " " << (mean / aoq) << " "
         << (ko_o.GetBrho() * (aoq / mean)) << std::endl;
  }
  fout.close();
}

void brho_cor_pid_o() { brho_cor_pid(true); }

void brho_cor_pid_ne() { brho_cor_pid(false); }