void pid_o() {
    KOBRA ko_o18(KOBRA::Expt::Phys, KOBRA::o18);
    ko_o18.SetBrho(1.3009);
    ko_o18.DrawPID(0);
    auto h_o18 = static_cast<TH2 *>(gDirectory->Get("hpid_r251"));

    KOBRA ko_o19(KOBRA::Expt::Phys, KOBRA::o19);
    ko_o19.SetBrho(1.3248);
    ko_o19.DrawPID(0);
    auto h_o19 = static_cast<TH2 *>(gDirectory->Get("hpid_r260"));
    KOBRA ko_o20(KOBRA::Expt::Phys, KOBRA::o20);
    ko_o20.SetBrho(1.3812);
    ko_o20.DrawPID(0);
    auto h_o20 = static_cast<TH2 *>(gDirectory->Get("hpid_r304"));

    KOBRA ko_o21(KOBRA::Expt::Phys, KOBRA::o21);
    ko_o21.SetBrho(1.3964);
    ko_o21.DrawPID(0);
    auto h_o21 = static_cast<TH2 *>(gDirectory->Get("hpid_r316"));

    KOBRA ko_o22(KOBRA::Expt::Phys, KOBRA::o22);
    ko_o22.SetBrho(1.4662);
    ko_o22.DrawPID(0);
    auto h_o22 = static_cast<TH2 *>(gDirectory->Get("hpid_r223"));

    auto c1 = new TCanvas("c1", "PID", 1600, 1200);
    auto h_sum = static_cast<TH2 *>(h_o18->Clone("h_sum"));
    h_sum->Add(h_o19);
    h_sum->Add(h_o20);
    h_sum->Add(h_o21);
    h_sum->Add(h_o22);

    h_sum->SetTitle("Summed PID for O18-O22;A/Q;Z");
    h_sum->Draw("colz");
}
