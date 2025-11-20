#ifndef __CLING__
#include "TCanvas.h"
#include "TF1.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void pid_reso() {
    // KOBRA ko_o20(KOBRA::Expt::Phys, KOBRA::o20);
    // ko_o20.SetBrho(1.3734);
    // ko_o20.DrawPID(0, "o20");
    // auto h_o20 = static_cast<TH2*>(gDirectory->Get("hpid_r304"));
    // auto hx = h_o20->ProjectionX();
    // auto hy = h_o20->ProjectionY();
    // hx->Fit("gaus");
    // hy->Fit("gaus");
    // TF1* f1 = hx->GetFunction("gaus");
    // TF1* f2 = hy->GetFunction("gaus");
    // std::cout << "A/Q resolution: " << f1->GetParameter(2) / f1->GetParameter(1) * 100 << " % in sigma" << std::endl;
    // std::cout << "A/Q resolution: " << f1->GetParameter(2) * 2.35 / f1->GetParameter(1) * 100 << " % in FWHM"
    //           << std::endl;
    // std::cout << "Z resolution: " << f2->GetParameter(2) / f2->GetParameter(1) * 100 << " % in sigma" << std::endl;
    // std::cout << "Z resolution: " << f2->GetParameter(2) * 2.35 / f2->GetParameter(1) * 100 << " % in FWHM"
    //           << std::endl;

    KOBRA ko_mdis_o20(KOBRA::Expt::Phys, KOBRA::mruns["o20"][0][0]);
    ko_mdis_o20.SetBrho(KOBRA::mruns_brho["o20"][0][0]);
    ko_mdis_o20.DrawPID(0, "o20");
    auto h_mdis_o20 = static_cast<TH2*>(gDirectory->Get("hpid_r601"));
    auto hx_mdis = h_mdis_o20->ProjectionX();
    auto hy_mdis = h_mdis_o20->ProjectionY();
    hx_mdis->Fit("gaus");
    hy_mdis->Fit("gaus");
    TF1* f1_mdis = hx_mdis->GetFunction("gaus");
    TF1* f2_mdis = hy_mdis->GetFunction("gaus");
    std::cout << "A/Q resolution: " << f1_mdis->GetParameter(2) / f1_mdis->GetParameter(1) * 100 << " % in sigma"
              << std::endl;
    std::cout << "A/Q resolution: " << f1_mdis->GetParameter(2) * 2.35 / f1_mdis->GetParameter(1) * 100 << " % in FWHM"
              << std::endl;
    std::cout << "Z resolution: " << f2_mdis->GetParameter(2) / f2_mdis->GetParameter(1) * 100 << " % in sigma"
              << std::endl;
    std::cout << "Z resolution: " << f2_mdis->GetParameter(2) * 2.35 / f2_mdis->GetParameter(1) * 100 << " % in FWHM"
              << std::endl;
}