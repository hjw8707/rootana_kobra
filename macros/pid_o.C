#ifndef __CLING__
#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void pid_o() {
    KOBRA ko_o18(KOBRA::Expt::Phys, KOBRA::o18);
    ko_o18.SetBrho(1.2959);
    ko_o18.DrawPID(0);
    ko_o18.DrawAllCuts(true);
    ko_o18.CountIsotopes();
    auto h_o18 = static_cast<TH2 *>(gDirectory->Get("hpid_r251"));
    gPad->GetCanvas()->Print("figs/o18.png");

    KOBRA ko_o19(KOBRA::Expt::Phys, KOBRA::o19);
    ko_o19.SetBrho(1.3198);
    ko_o19.DrawPID(0);
    ko_o19.DrawAllCuts(true);
    ko_o19.CountIsotopes();
    auto h_o19 = static_cast<TH2 *>(gDirectory->Get("hpid_r259"));
    gPad->GetCanvas()->Print("figs/o19.png");

    KOBRA ko_o20(KOBRA::Expt::Phys, KOBRA::o20);
    ko_o20.SetBrho(1.3734);
    ko_o20.DrawPID(0);
    ko_o20.DrawAllCuts(true);
    ko_o20.CountIsotopes();
    auto h_o20 = static_cast<TH2 *>(gDirectory->Get("hpid_r304"));
    gPad->GetCanvas()->Print("figs/o20.png");

    KOBRA ko_o21(KOBRA::Expt::Phys, KOBRA::o21);
    ko_o21.SetBrho(1.3879);
    ko_o21.DrawPID(0);
    ko_o21.DrawAllCuts(true);
    ko_o21.CountIsotopes();
    auto h_o21 = static_cast<TH2 *>(gDirectory->Get("hpid_r316"));
    gPad->GetCanvas()->Print("figs/o21.png");

    KOBRA ko_o22(KOBRA::Expt::Phys, KOBRA::o22);
    ko_o22.SetBrho(1.4600);
    ko_o22.DrawPID(0);
    ko_o22.DrawAllCuts(true);
    ko_o22.CountIsotopes();
    auto h_o22 = static_cast<TH2 *>(gDirectory->Get("hpid_r223"));
    gPad->GetCanvas()->Print("figs/o22.png");

    auto c1 = new TCanvas("c1", "PID", 1600, 1200);
    auto h_sum = static_cast<TH2 *>(h_o18->Clone("h_sum"));
    h_sum->Add(h_o19);
    h_sum->Add(h_o20);
    h_sum->Add(h_o21);
    h_sum->Add(h_o22);

    h_sum->SetTitle("Summed PID for O18-O22;A/Q;Z");
    h_sum->Draw("colz");
    ko_o22.DrawAllCuts(true);
    gPad->GetCanvas()->Print("figs/o_sum.png");
}
