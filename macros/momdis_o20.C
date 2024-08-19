{
  KOBRA ko(572,575);
  ko.SetBrho(1.4048);
  ko.SetUsePla();
  ko.ApplyOffsetToCut(-0.025,-0.2,-1,1.2);
  ko.DrawMomDist("o20", true);

  KOBRA ko2(576,584);
  ko2.SetBrho(1.5065);
  ko2.SetUsePla();
  ko2.ApplyOffsetToCut(-0.025,-0.2,-1,1.2);
  ko2.DrawMomDist("o20", true);

  KOBRA ko3(590,595);
  ko3.SetBrho(1.5065);
  ko3.SetUsePla();
  ko3.ApplyOffsetToCut(-0.025,-0.2,-1,1.2);
  ko3.DrawMomDist("o20", true);
  
  auto h1 = static_cast<TH1*>(gDirectory->Get("hmd_r572_o20"));
  auto h2 = static_cast<TH1*>(gDirectory->Get("hmd_r576_o20"));
  auto h3 = static_cast<TH1*>(gDirectory->Get("hmd_r590_o20"));
    
  TGraphErrors *gr1 = new TGraphErrors;
  gr1->SetTitle("Momentum Distribution of ^{20}O;Momentum Dispersion (%);Rate (pps)");
  for (Int_t i = 1 ; i <= h1->GetNbinsX() ; i++) {
    gr1->AddPoint(-4.25+i*0.5, h1->GetBinContent(i));
    gr1->SetPointError(gr1->GetN()-1,0.25,  h1->GetBinError(i));
  }

  TGraphErrors *gr2 = new TGraphErrors;
  for (Int_t i = 1 ; i <= h2->GetNbinsX() ; i++) {
    gr2->AddPoint(-4.25+7+i*0.5, h2->GetBinContent(i));
    gr2->SetPointError(gr2->GetN()-1,
		       0.25,
		       h2->GetBinError(i));
  }
  TGraphErrors *gr3 = new TGraphErrors;
  for (Int_t i = 1 ; i <= h3->GetNbinsX() ; i++) {
    gr3->AddPoint(-4.25+7+i*0.5, h3->GetBinContent(i));
    gr3->SetPointError(gr3->GetN()-1,
  		       0.25,
  		       h3->GetBinError(i));
  }

  gr1->SetMarkerStyle(20);
  gr1->SetMarkerColor(1);
  gr1->SetMarkerSize(2);
  gr2->SetMarkerStyle(22);
  gr2->SetMarkerColor(2);
  gr2->SetMarkerSize(2);

  gr3->SetMarkerStyle(25);
  gr3->SetMarkerColor(4);
  gr3->SetMarkerSize(2);
  
  gr1->GetXaxis()->SetLimits(-4,8);
  gr1->GetXaxis()->SetRangeUser(-4,8);
  gr1->GetYaxis()->SetLimits(0,0.2);
  gr1->GetYaxis()->SetRangeUser(0,0.2);

  gr1->Draw("AP");
  gr2->Draw("P");
  gr3->Draw("P");
}
