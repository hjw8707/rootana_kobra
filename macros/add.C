{
    Double_t binSize = 1;
    const char* isotope = "o19";

    KOBRA ko(601,604);
    ko.SetBrho(1.4048);
    ko.SetUsePla();
    ko.ApplyOffsetToCut(0,0,-1,-1);
    ko.DrawPIDC(2,isotope);

    KOBRA ko2(590,598);
    ko2.SetBrho(1.5065);
    ko2.SetUsePla();
    ko2.ApplyOffsetToCut(-0.005,0.2,-1,0.8);
    ko2.DrawPIDC(2,isotope);

    KOBRA ko3(605,607);
    ko3.SetBrho(1.4636);
    ko3.SetUsePla();
    ko3.ApplyOffsetToCut(0,0.3,-1,0.8);
    ko3.DrawPIDC(2,isotope);

    KOBRA ko4(610,612);
    ko4.SetBrho(1.3510);
    ko4.SetUsePla();
    ko4.ApplyOffsetToCut(0.015,-0.1,-1,-1);
    ko4.DrawPIDC(2,isotope);

    KOBRA ko5(615,617);
    ko5.SetBrho(1.3229);
    ko5.SetUsePla();
    ko5.ApplyOffsetToCut(0.02,-0.1,-1,-1);
    ko5.DrawPIDC(2,isotope);

    KOBRA ko6(620,625);
    ko6.SetBrho(1.2947);
    ko6.SetUsePla();
    ko6.ApplyOffsetToCut(0.025,-0.15,-1,-1);
    ko6.DrawPIDC(2,isotope);

    auto h1 = static_cast<TH2*>(gDirectory->Get("hpidc_r601"));
    auto h2 = static_cast<TH2*>(gDirectory->Get("hpidc_r590"));
    auto h3 = static_cast<TH2*>(gDirectory->Get("hpidc_r605"));
    auto h4 = static_cast<TH2*>(gDirectory->Get("hpidc_r610"));
    auto h5 = static_cast<TH2*>(gDirectory->Get("hpidc_r615"));
    auto h6 = static_cast<TH2*>(gDirectory->Get("hpidc_r620"));

    auto hadd = static_cast<TH2*> (h1 -> Clone());
    hadd -> Add(h2);
    hadd -> Add(h3);
    hadd -> Add(h4);
    hadd -> Add(h5);
    hadd -> Add(h6);
    hadd -> Draw("colz");
}
