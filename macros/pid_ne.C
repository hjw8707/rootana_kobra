#ifndef __CLING__
#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void pid_ne() {
    KOBRA ko_ne24(KOBRA::Expt::Phys, KOBRA::ne24);
    ko_ne24.SetBrho(1.2646443);
    ko_ne24.DrawPID(0);
    ko_ne24.DrawAllCuts(true);
    ko_ne24.CountIsotopes();
    auto h_ne24 = static_cast<TH2 *>(gDirectory->Get("hpid_r410"));
    gPad->GetCanvas()->Print("figs/ne24.png");

    KOBRA ko_ne25(KOBRA::Expt::Phys, KOBRA::ne25);
    ko_ne25.SetBrho(1.3201144);
    ko_ne25.DrawPID(0);
    ko_ne25.DrawAllCuts(true);
    ko_ne25.CountIsotopes();
    auto h_ne25 = static_cast<TH2 *>(gDirectory->Get("hpid_r420"));
    gPad->GetCanvas()->Print("figs/ne25.png");

    KOBRA ko_ne26(KOBRA::Expt::Phys, KOBRA::ne26);
    ko_ne26.SetBrho(1.3741834);
    ko_ne26.DrawPID(0);
    ko_ne26.DrawAllCuts(true);
    ko_ne26.CountIsotopes();
    auto h_ne26 = static_cast<TH2 *>(gDirectory->Get("hpid_r425"));
    gPad->GetCanvas()->Print("figs/ne26.png");

    auto c1 = new TCanvas("c1", "PID", 1600, 1200);
    auto h_sum = static_cast<TH2 *>(h_ne24->Clone("h_sum"));
    h_sum->Add(h_ne25);
    h_sum->Add(h_ne26);

    h_sum->SetTitle("Summed PID for Ne24-Ne26;A/Q;Z");
    h_sum->Draw("colz");

    ko_ne26.DrawAllCuts(true);
    gPad->GetCanvas()->Print("figs/ne_sum.png");
}
