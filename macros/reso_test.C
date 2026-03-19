#ifndef __CLING__
#include <iomanip>

#include "TCanvas.h"
#include "TEventList.h"
#include "TF1.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void reso_test() {
  KOBRA ko_o20(KOBRA::Expt::Phys, KOBRA::csruns["o20"], KOBRA::brhoValue["o20"]);
  ko_o20.tree->Draw(">>e_o20", "o20");
  TEventList* o20_list = static_cast<TEventList*>(gDirectory->Get("e_o20"));
  Double_t brho = ko_o20.GetBrho();

  const Int_t nBrho = 7;
  const Int_t nTOF = 7;
  Double_t rBrho = 0.005;  // ratio from the central brho
  Double_t rTOF = 1;       // ns from the original TOF
  ko_o20.tree->SetEventList(o20_list);

  TCanvas* c1 = new TCanvas("c1", "reso_test", 1000, 1000);
  c1->Divide(nBrho, nTOF);

  Double_t sigma[nBrho][nTOF];
  Double_t mean[nBrho][nTOF];
  for (Int_t iBrho = 0; iBrho < nBrho; iBrho++) {
    ko_o20.SetBrho(brho * (1 + rBrho * (iBrho - nBrho / 2)));
    for (Int_t iTOF = 0; iTOF < nTOF; iTOF++) {
      ko_o20.SetTOFOffset(rTOF * (iTOF - nTOF / 2));

      c1->cd(iBrho * nTOF + iTOF + 1);
      ko_o20.tree->Draw(Form("aoqcorp>>h_aoq_r%d_t%d(400)", iBrho, iTOF));
      auto h_aoq = static_cast<TH1*>(gDirectory->Get(Form("h_aoq_r%d_t%d", iBrho, iTOF)));
      h_aoq->Fit("gaus");
      TF1* f1 = h_aoq->GetFunction("gaus");
      sigma[iBrho][iTOF] = f1->GetParameter(2);
      mean[iBrho][iTOF] = f1->GetParameter(1);
    }
  }

  std::cout << "         , ";
  for (Int_t iTOF = 0; iTOF < nTOF; iTOF++) std::cout << std::setw(4) << rTOF * (iTOF - nTOF / 2) << " ns, ";
  std::cout << std::endl;
  for (Int_t iBrho = 0; iBrho < nBrho; iBrho++) {
    std::cout << std::fixed << std::setprecision(5) << brho * (1 + rBrho * (iBrho - nBrho / 2)) << "Tm, ";
    for (Int_t iTOF = 0; iTOF < nTOF; iTOF++) {
      std::cout << std::fixed << std::setprecision(4) << sigma[iBrho][iTOF] / mean[iBrho][iTOF] * 100. << "%, ";
    }
    std::cout << std::endl;
  }
}