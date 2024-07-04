void setbrho(double brho) {
}

void setalias(TChain *tree = NULL) {
  if (!tree) tree = static_cast<TChain*>(gDirectory->Get("kobra"));
  if (!tree) return;
  
  std::cout << "Set Alias" << std::endl;
  tree->SetAlias("f1a", "(f1dppac.x - f1uppac.x)/527*1000.");
  tree->SetAlias("f1b", "(f1dppac.y - f1uppac.y)/527*1000.");
  tree->SetAlias("f2a", "(f2dppac.x - f2uppac.x)/480*1000.");
  tree->SetAlias("f2b", "(f2dppac.y - f2uppac.y)/480*1000.");
  tree->SetAlias("f3a", "(f3dppac.x - f3uppac.x)/570*1000.");
  tree->SetAlias("f3b", "(f3dppac.y - f3uppac.y)/570*1000.");

  tree->SetAlias("db", "f1uppac.x");
  tree->SetAlias("tof", "(f3uppt.tsum - f2dppt.tsum) + 103.8");
  tree->SetAlias("de", "Max$(f2ssd.acal)");

  tree->SetAlias("brho", "(1+db/4100)*1.3254");
  //tree->SetAlias("brho", "1.3254");

  tree->SetAlias("beta", "(35526.1-23127.6)/tof/299.");
  tree->SetAlias("gamma", "1/sqrt(1-beta*beta)");
  tree->SetAlias("pp", "gamma*beta");

  tree->SetAlias("aoq", "brho/(3.1*pp)");
  tree->SetAlias("aoqc", "aoq-0.0025*(tof-250)");
  //tree->SetAlias("AoQ", "1.333*(aoqc-2.32) + 2.19"); for run402
  tree->SetAlias("AoQ", "(aoqc-2.15)*1.3 + 1.99");
  tree->SetAlias("corfac", "log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
  tree->SetAlias("z", "sqrt(de)*beta/sqrt(corfac)");
  tree->SetAlias("zc","z-1.4*(aoqc-2.15)");

  tree->SetAlias("Z","(z-0.325)/0.045+4.8");
}

TChain* chain(int runs, int rune = 0) {
    // TChain 객체 생성
    TChain* chain = new TChain("kobra");


    if (rune == 0) {
      chain->Add(Form("root/output%08d.root", runs));
      setalias(chain);
      return chain;}

    for (int r = runs ; r <= rune ; r++) {
      std::cout << "Run : " << r << std::endl;
      chain->Add(Form("root/output%08d.root", r));
    }
    setalias(chain);
    return chain; }
