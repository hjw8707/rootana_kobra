#ifndef __CLING__
#include <fstream>

#include "TCanvas.h"
#include "TDatime.h"
#include "TFile.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void count_pid(bool flag_o = true) {
  int n_mass = flag_o ? 5 : 3;
  int o_mass[] = {18, 19, 20, 21, 22};
  int ne_mass[] = {24, 25, 26};
  std::vector<TH2 *> h_pid_list;

  TDatime datime;
  std::string date_str = std::to_string(datime.GetDate());

  std::ofstream fout(Form("analysis/count_pid_counts_%s_cor_%s.txt", flag_o ? "o" : "ne", date_str.c_str()));
  for (int i = 0; i < n_mass; i++) {
    std::string mass_str = flag_o ? "o" + std::to_string(o_mass[i]) : "ne" + std::to_string(ne_mass[i]);
    std::string title_str = flag_o ? std::to_string(o_mass[i]) + "O" : std::to_string(ne_mass[i]) + "Ne";
    KOBRA ko_o(KOBRA::Expt::Phys, KOBRA::csruns[mass_str], KOBRA::brhoValue[mass_str]);
    ko_o.SetStyleAhn();
    auto h_pid = ko_o.DrawCorPID(title_str.c_str());
    // ko_o.DrawAllCuts(true);
    ko_o.CountIsotopesRunByRun(fout, false, i > 0 ? false : true);
    gPad->GetCanvas()->Print(Form("figs/%s_corp.pdf", mass_str.c_str()));
    h_pid->SetDirectory(nullptr);
    h_pid_list.push_back(h_pid);
  }

  auto c1 = new TCanvas("c1", "PID", 1600, 1200);
  auto h_sum = static_cast<TH2 *>(h_pid_list[0]->Clone("h_sum"));
  for (int i = 1; i < n_mass; i++) {
    h_sum->Add(h_pid_list[i]);
  }
  KOBRA *ko = new KOBRA(KOBRA::Expt::Phys, KOBRA::csruns["o18"], KOBRA::brhoValue["o18"]);
  h_sum->SetTitle(Form("Summed PID for %s;A/Q;Z", flag_o ? "O18-O22" : "Ne24-Ne26"));
  h_sum->Draw("colz");
  // ko->DrawAllCuts(true);
  gPad->GetCanvas()->Print(Form("figs/%s_sum_cor_%s.pdf", flag_o ? "o" : "ne", date_str.c_str()));
  fout.close();

  TFile *f = new TFile(Form("analysis/count_pid_cor_%s.root", date_str.c_str()), "UPDATE");
  for (int i = 0; i < n_mass; i++) {
    h_pid_list[i]->Write();
  }
  h_sum->SetName(Form("h_sum_%s", flag_o ? "o" : "ne"));
  h_sum->Write();
  f->Close();
}

void count_pid_o() { count_pid(true); }

void count_pid_ne() { count_pid(false); }