void pid() {
    KOBRA ko_ne24(KOBRA::Expt::Phys, KOBRA::ne24);
    ko_ne24.SetBrho(1.2677);
    ko_ne24.DrawPID(0);
    auto h_ne24 = static_cast<TH2 *>(gDirectory->Get("hpid_r410"));

    KOBRA ko_ne25(KOBRA::Expt::Phys, KOBRA::ne25);
    ko_ne25.SetBrho(1.326);
    ko_ne25.DrawPID(0);
    auto h_ne25 = static_cast<TH2 *>(gDirectory->Get("hpid_r420"));

    KOBRA ko_ne26(KOBRA::Expt::Phys, KOBRA::ne26);
    ko_ne26.SetBrho(1.3814);
    ko_ne26.DrawPID(0);
    auto h_ne26 = static_cast<TH2 *>(gDirectory->Get("hpid_r430"));

    // auto c1 = new TCanvas("c1", "PID", 1600, 1200);
    // h_ne24->Draw("col");
}
