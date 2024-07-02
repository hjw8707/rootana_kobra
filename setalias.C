{
  kobra->SetAlias("db","f1uppac.x");
  kobra->SetAlias("tof","(f3uppt.tsum - f2dppt.tsum) + 103.8");
  kobra->SetAlias("de","Sum$(f2ssd.acal)");

  kobra->SetAlias("brho","(1+db/4100)*1.1");

  kobra->SetAlias("beta","(35526.1-23127.6)/tof/299.");
  kobra->SetAlias("gamma","1/sqrt(1-beta*beta)");
  kobra->SetAlias("pp","gamma*beta");

  kobra->SetAlias("aoq","brho/(3.1*pp)");

  kobra->SetAlias("corfac","log(2*511./0.173*beta*beta/gamma/gamma) - beta*beta");
  kobra->SetAlias("z","sqrt(de)*tof/sqrt(corfac)");
}
