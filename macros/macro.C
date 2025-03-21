#ifndef __CLING__
#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void macro() {
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::o19);
    ko.SetBrho(1.3198);
    ko.DrawPID(0);

    TString cut = "aoq > 2.353 && aoq < 2.395";

    TCanvas *c1 = new TCanvas("c1", "PID", 1600, 1200);
    c1->Divide(7, 4);

    TString yaxis = "Sum$(f3ssd.adc)";
    TString xaxis[28] = {"f1uppac.tx1", "f1uppac.tx2", "f1uppac.ty1", "f1uppac.ty2", "f1uppac.ta",  "f1uppac.x",
                         "f1uppac.y",   "f2dppac.tx1", "f2dppac.tx2", "f2dppac.ty1", "f2dppac.ty2", "f2dppac.ta",
                         "f2dppac.x",   "f2dppac.y",   "f3uppac.tx1", "f3uppac.tx2", "f3uppac.ty1", "f3uppac.ty2",
                         "f3uppac.ta",  "f3uppac.x",   "f3uppac.y",   "f3dppac.tx1", "f3dppac.tx2", "f3dppac.ty1",
                         "f3dppac.ty2", "f3dppac.ta",  "f3dppac.x",   "f3dppac.y"};

    for (int i = 0; i < 28; i++) {
        c1->cd(i + 1);
        ko.tree->Draw(Form(yaxis + ":" + xaxis[i] + ">>h%d", i), cut);
        TH2 *h = (TH2 *)gDirectory->FindObject(Form("h%d", i));
        h->Draw("colz");
    }
}
