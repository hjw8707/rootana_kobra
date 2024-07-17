void DrawCanvas(int run, KOBRA &ko, const char* cut) {
  TString cname = "can";
  if (cut)
    cname += cut;

  TCanvas *c1 = new TCanvas(cname.Data(),"Shift Monitor",1200,900);
  if (cut)
    c1->SetTitle(Form("Shift Monitor %s",cut));
   
  c1->Divide(3,3);
  c1->cd(9);
  TText text;
  text.SetTextFont(63);
  text.SetTextSize(50);
  text.SetTextAlign(22);
  text.DrawTextNDC(0.5,0.5,Form("Run: %05d",run));
  
  auto vpad = c1->cd(1);
  ko.tree->Draw("Z:aoq>>h1(200,2.0,3.0,200,2,12)",cut,"col");
  auto h1 = static_cast<TH1*>(gDirectory->Get("h1"));
  if (cut)  h1->SetName(Form("h1_%s",cut));
  else h1->SetName("h1_all");
  vpad->SetLogz();
  vpad->SetGridx();
  vpad->SetGridy();

  vpad = c1->cd(2);
  ko.tree->Draw("de:tof>>h2(200,180,280,200,0,50)",cut,"col");
  auto h2 = static_cast<TH2*>(gDirectory->Get("h2"));
  if (cut)  h2->SetName(Form("h2_%s",cut));
  else  h2->SetName("h2_all");
  vpad->SetLogz();  
  vpad->SetGridx();
  vpad->SetGridy();
  
  vpad = c1->cd(3);
  ko.tree->Draw("f3ssd.acal:f3ssd.ch>>h3(16,0,16,420,0,50)",cut,"col");
  h2 = static_cast<TH2*>(gDirectory->Get("h3"));
  if (cut) h2->SetName(Form("h3_%s",cut));
  else h2->SetName("h3_all");
  vpad->SetLogz();
  vpad->SetGridx();
  vpad->SetGridy();
  
  vpad = c1->cd(4);
  ko.tree->Draw("f1uppac.y:f1uppac.x>>hf1(200,-200,200,200,-100,100)",cut,"col");
  h2 = static_cast<TH2*>(gDirectory->Get("hf1"));
  if (cut) h2->SetName(Form("hf1_%s",cut));
  else h2->SetName("hf1_all");
  vpad->SetLogz();
  vpad->SetGridx();
  vpad->SetGridy();
  
  vpad = c1->cd(5);
  ko.tree->Draw("f2dppac.y:f2dppac.x>>hf2(200,-100,100,200,-100,100)",cut,"col");
  h2 = static_cast<TH2*>(gDirectory->Get("hf2"));
  if (cut) h2->SetName(Form("hf2_%s",cut));
  else h2->SetName("hf2_all");
  vpad->SetLogz();
  vpad->SetGridx();
  vpad->SetGridy();
  
  vpad = c1->cd(6);
  ko.tree->Draw("f3y:f3x>>hf3(200,-100,100,200,-100,100)",cut,"col");
  h2 = static_cast<TH2*>(gDirectory->Get("hf3"));
  if (cut) h2->SetName(Form("hf3_%s",cut));
  else h2->SetName("hf3_all");
  vpad->SetLogz();
  vpad->SetGridx();
  vpad->SetGridy();

  vpad = c1->cd(7);
  ko.tree->Draw("f3x>>hf3x(200,-100,100)",cut);
  h1 = static_cast<TH1*>(gDirectory->Get("hf3x"));
  if (cut) h1->SetName(Form("hf3x_%s",cut));
  else h1->SetName("hf3x_all");

  vpad = c1->cd(8);
  ko.tree->Draw("f3y>>hf3y(200,-100,100)",cut);
  h1 = static_cast<TH1*>(gDirectory->Get("hf3y"));
  if (cut) h1->SetName(Form("hf3y_%s",cut));
  else h1->SetName("hf3y_all");
  
  if (cut) {
    c1->SaveAs(Form("figs/run%05d_%s.pdf",run,cut));
    c1->SaveAs(Form("figs/run%05d_%s.png",run,cut));}
  else {
    c1->SaveAs(Form("figs/run%05d_all.pdf",run));
    c1->SaveAs(Form("figs/run%05d_all.png",run));}
}

void shift(int run) {

  double brho = 1.3520;
  
  KOBRA ko(run);
  ko.SetUseF1();
  ko.SetBrho(brho);
  ko.ApplyOffsetToCut(-0.005);
  
  //  gStyle->SetPalette(kInvertedDarkBodyRadiator);
  gStyle->SetPalette(kDeepSea);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
    
  DrawCanvas(run, ko, 0);
  //  DrawCanvas(run, ko, "o17");
  //  DrawCanvas(run, ko, "o18");
  DrawCanvas(run, ko, "o19");
  DrawCanvas(run, ko, "o20");
  DrawCanvas(run, ko, "o21");
  DrawCanvas(run, ko, "o22");

  ////////////////////////////////////////////////////////////
  // Efficiency
  Int_t f1ussdppac = ko.tree->GetEntries("f3ssd@.GetEntriesFast() > 0 && f1uppacx@.GetEntriesFast() > 0");
  Int_t f2dssdppac = ko.tree->GetEntries("f3ssd@.GetEntriesFast() > 0 && f2dppac@.GetEntriesFast() > 0");
  Int_t f3ussdppac = ko.tree->GetEntries("f3ssd@.GetEntriesFast() > 0 && f3uppac@.GetEntriesFast() > 0");
  Int_t f3dssdppac = ko.tree->GetEntries("f3ssd@.GetEntriesFast() > 0 && f3dppac@.GetEntriesFast() > 0");  
  Int_t f3ssd = ko.tree->GetEntries("f3ssd@.GetEntriesFast() > 0");

  Double_t eff_f1u = (double)f1ussdppac/f3ssd;
  Double_t eff_f2d = (double)f2dssdppac/f3ssd;
  Double_t eff_f3u = (double)f3ussdppac/f3ssd;
  Double_t eff_f3d = (double)f3dssdppac/f3ssd;

  std::cout << "=========================================" << std::endl;
  std::cout << "   Efficiencies of PPACs " << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << " F1U(X only): " << eff_f1u << std::endl;
  std::cout << " F2D        : " << eff_f2d << std::endl;
  std::cout << " F3U        : " << eff_f3u << std::endl;
  std::cout << " F3D        : " << eff_f3d << std::endl;
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////

  Int_t count17o = ko.tree->GetEntries("o17");
  Int_t count18o = ko.tree->GetEntries("o18");
  Int_t count19o = ko.tree->GetEntries("o19");
  Int_t count20o = ko.tree->GetEntries("o20");
  Int_t count21o = ko.tree->GetEntries("o21");
  Int_t count22o = ko.tree->GetEntries("o22");
  ////////////////////////////////////////////////////////////
  // Counting
  std::cout << "=========================================" << std::endl;
  std::cout << "   Counts of Isotopes " << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << " 17O: " << count17o << std::endl;
  std::cout << " 18O: " << count18o << std::endl;
  std::cout << " 19O: " << count19o << std::endl;
  std::cout << " 20O: " << count20o << std::endl;
  std::cout << " 21O: " << count21o << std::endl;
  std::cout << " 22O: " << count22o << std::endl;
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////

  Double_t time = ko.GetElapsedTime();
  ////////////////////////////////////////////////////////////
  // Counting
  std::cout << "=========================================" << std::endl;
  std::cout << "   Rates of Isotopes " << std::endl;
  std::cout << "=========================================" << std::endl;
  std::cout << " Elapsed Time: " << time << " sec." << std::endl;
  std::cout.precision(4);
  std::cout << " 17O: " << static_cast<Double_t>(count17o)/time << std::endl;
  std::cout << " 18O: " << static_cast<Double_t>(count18o)/time << std::endl;
  std::cout << " 19O: " << static_cast<Double_t>(count19o)/time << std::endl;
  std::cout << " 20O: " << static_cast<Double_t>(count20o)/time << std::endl;
  std::cout << " 21O: " << static_cast<Double_t>(count21o)/time << std::endl;
  std::cout << " 22O: " << static_cast<Double_t>(count22o)/time << std::endl;
  std::cout << "=========================================" << std::endl;
  ////////////////////////////////////////////////////////////  
  
}
