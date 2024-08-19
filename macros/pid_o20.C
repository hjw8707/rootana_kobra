void pid_o20(int rStart, int rEnd){
  KOBRA ko(rStart, rEnd);
  ko.SetBrho(1.4073);
  ko.SetUsePla();
  ko.ApplyOffsetToCut(-0.025,-0.2,-1,1.2);
  ko.DrawPIDC(2);
  ko.DrawXDist("o20");
}
