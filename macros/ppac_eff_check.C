#ifndef __CLING__
#include "TBox.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLine.h"
#include "kobra.hxx"
#endif

void ppac_eff_check() {
  const char* isos[] = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};

  TCanvas* c1 = new TCanvas("c1", "PPAC Efficiency Check", 1200, 900);
  c1->Divide(3, 3);
  int i = 1;
  for (const char* iso : isos) {
    c1->cd(i++);
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns[iso], KOBRA::brhoValue[iso]);
    std::cout << iso << " Runs" << std::endl;
    ko.DrawPPACMultiHits(0, true);
  }
}