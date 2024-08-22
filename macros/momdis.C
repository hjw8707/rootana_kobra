{
  Double_t binSize = 0.5;
  const char* isotope = "o20";

  KOBRA ko(601,604);
  ko.SetBrho(1.4048);
  ko.SetUsePla();
  ko.ApplyOffsetToCut(0,0,-1,-1);
  auto gr1 = ko.GetMomDistGraph(0, isotope, binSize, 3);

  //  KOBRA ko2(576,579);
  KOBRA ko2(590,598);
  ko2.SetBrho(1.5065);
  ko2.SetUsePla();
  ko2.ApplyOffsetToCut(-0.005,0.2,-1,0.8);
  auto gr2 = ko2.GetMomDistGraph(7, isotope, binSize, 3);

  KOBRA ko3(605,607);
  ko3.SetBrho(1.4636);
  ko3.SetUsePla();
  ko3.ApplyOffsetToCut(0,0.3,-1,0.8);
  auto gr3 = ko3.GetMomDistGraph(4, isotope, binSize, 3);

  KOBRA ko4(610,612);
  ko4.SetBrho(1.3510);
  ko4.SetUsePla();
  ko4.ApplyOffsetToCut(0.015,-0.1,-1,-1);
  auto gr4 = ko4.GetMomDistGraph(-4, isotope, binSize, 1);

  KOBRA ko5(615,617);
  ko5.SetBrho(1.3229);
  ko5.SetUsePla();
  ko5.ApplyOffsetToCut(0.02,-0.1,-1,-1);
  auto gr5 = ko5.GetMomDistGraph(-6, isotope, binSize, 0.5);

  KOBRA ko6(620,625);
  ko6.SetBrho(1.2947);
  ko6.SetUsePla();
  ko6.ApplyOffsetToCut(0.025,-0.15,-1,-1);
  auto gr6 = ko6.GetMomDistGraph(-8, isotope, binSize, 0.5);

  KOBRA ko7(627, 630);
  ko7.SetBrho(1.2666);
  ko7.SetUsePla();
  ko7.ApplyOffsetToCut(0.04);
  auto gr7 = ko7.GetMomDistGraph(-10, isotope, binSize, 1);
  
  for (Int_t i = 0 ; i < gr7->GetN() ; i++)
    gr7->GetY()[i] *= 1.735;


      
  KOBRA ko8(vector<int>{632,636});
  ko8.SetBrho(1.4087);
  ko8.SetUsePla();
  ko8.ApplyOffsetToCut(0.01);
  auto gr8 = ko8.GetMomDistGraph(0, isotope, binSize);
  for (Int_t i = 0 ; i < gr8->GetN() ; i++)
    gr8->GetY()[i] *= 1.466;

    
  KOBRA ko9(639,642);
  ko9.SetBrho(1.2666);
  ko9.SetUsePla();
  ko9.ApplyOffsetToCut(0.04);
  auto gr9 = ko9.GetMomDistGraph(-10, isotope, 0.2, 0.1, true);

    
  KOBRA koa(643,648);
  koa.SetBrho(1.2384);
  koa.SetUsePla();
  koa.ApplyOffsetToCut(0.05);
  auto gra = koa.GetMomDistGraph(-12, isotope, 0.15, 0.05, true);

      
  KOBRA kob(651, 655);
  kob.SetBrho(1.3088);
  kob.SetUsePla();
  kob.ApplyOffsetToCut(0.03);
  auto grb = kob.GetMomDistGraph(-7, isotope, 0.5, 0.5);

        
  KOBRA koc(656,657);
  koc.SetBrho(1.2064);
  koc.SetUsePla();
  koc.ApplyOffsetToCut(0.04);
  auto grc = koc.GetMomDistGraph(-14, isotope, 0.1, 0.1);

  KOBRA kod(660,661);
  kod.SetBrho(1.3369);
  kod.SetUsePla();
  kod.ApplyOffsetToCut(0.02);
  auto grd = kod.GetMomDistGraph(-5 , isotope, 0.5,1  );
  
  gr1->SetMarkerStyle(20);
  gr1->SetMarkerColor(1);
  gr2->SetMarkerStyle(21);
  gr2->SetMarkerColor(2);
  gr3->SetMarkerStyle(22);
  gr3->SetMarkerColor(3);
  gr4->SetMarkerStyle(23);
  gr4->SetMarkerColor(4);
  gr5->SetMarkerStyle(24);
  gr5->SetMarkerColor(6);
  gr6->SetMarkerStyle(25);
  gr6->SetMarkerColor(7);
  gr7->SetMarkerStyle(26);
  gr7->SetMarkerColor(8);  
  gr8->SetMarkerStyle(27);
  gr8->SetMarkerColor(9);  
  gr9->SetMarkerStyle(29);
  gr9->SetMarkerColor(11);
  gra->SetMarkerStyle(30);
  gra->SetMarkerColor(12);
  grb->SetMarkerStyle(31);
  grb->SetMarkerColor(13);
  grc->SetMarkerStyle(32);
  grc->SetMarkerColor(14);
  grd->SetMarkerStyle(33);
  grd->SetMarkerColor(15); 
  gr1->GetXaxis()->SetLimits(-16,10);
  gr1->GetXaxis()->SetRangeUser(-16,10);
  gr1->GetYaxis()->SetLimits(0,22);
  gr1->GetYaxis()->SetRangeUser(0,0.7);

  TCanvas *c3 = new TCanvas("cmdis_all","Mom. Distribution",1000,800);
  gr1->Draw("AP");
  gr2->Draw("P");
  gr3->Draw("P");
  gr4->Draw("P");
  gr5->Draw("P");
  gr6->Draw("P");
  //  gr7->Draw("P");
  //  gr8->Draw("P");
  gr9->Draw("P");
  gra->Draw("P");
  grb->Draw("P");
  grc->Draw("P");
  grd->Draw("P");

  /*  ko.DrawMomDist(isotope, false, true, binSize);
      ko2.DrawMomDist(isotope, false, true, binSize);
      ko3.DrawMomDist(isotope, false, true, binSize);
      ko4.DrawMomDist(isotope, false, true, binSize);
      ko5.DrawMomDist(isotope, false, true, binSize);*/
  
  //  kob.DrawMomDist(isotope, false, true, binSize);*/

  vector<TGraphErrors*> vec_gr;
  vec_gr.push_back(gr1);
  vec_gr.push_back(gr2);
  vec_gr.push_back(gr3);
  vec_gr.push_back(gr4);
  vec_gr.push_back(gr5);
  vec_gr.push_back(gr6);
  //  vec_gr.push_back(gr7);
  //  vec_gr.push_back(gr8);
  vec_gr.push_back(gr9);
  vec_gr.push_back(gra);
  vec_gr.push_back(grb);
  vec_gr.push_back(grc);
  vec_gr.push_back(grd);

  TGraphErrors* gr_all = new TGraphErrors;
  for (const auto & it: vec_gr) {
    for (Int_t i = 0 ; i < it->GetN() ; i++) {
      gr_all->AddPoint(it->GetX()[i],it->GetY()[i]);
      gr_all->SetPointError(gr_all->GetN()-1, it->GetEX()[i], it->GetEY()[i]);
    }
  }
  /*
  gr_all->Draw("AP");
  TF1 *f1 = new TF1("f1","gaus",-15,15);
  f1->SetParameters(0.526733, -2.17686, 4.67626);
  for (int i = 0 ; i < 100 ; i++)
  gr_all->Fit(f1);
  */
}
