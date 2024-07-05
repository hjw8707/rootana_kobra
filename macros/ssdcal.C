void ssdcal(TChain *chain){
  if (!chain) return;

  const int n = 16;
  double range = 20;
  
  TSpectrum ts;
  std::vector<std::vector<double>> allpeaks;
  for (int i = 0 ; i < n ; i++) {
    chain->Draw(Form("f3ssd.adc>>hssd%02i(500,0,1000)",i),
		Form("f3ssd.ch == %i",i));

    TH1 *h1;
    h1 = static_cast<TH1*>(gDirectory->Get(Form("hssd%02i",i)));

    
    h1->Draw();

    ts.Search(h1,5,"",0.0005);
    std::vector<double> peaks;
    for (int j = 0 ; j < ts.GetNPeaks() ;j++) {
      h1->Fit("gaus",0,0,
	      ts.GetPositionX()[j] - range,
	      ts.GetPositionX()[j] + range);
      TF1 *f1 = h1->GetFunction("gaus");
      peaks.push_back(f1->GetParameter(1));
      gPad->GetCanvas()->Update();
      gPad->SetLogy();
      getchar();
    }
    sort(peaks.begin(), peaks.end());
    
    allpeaks.push_back(peaks);
  }


  for (const auto &it : allpeaks){
    for (const auto &it2 : it){
      std::cout << it2 << " ";
    }
    std::cout << std::endl;
  }

}
