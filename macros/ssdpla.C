#include <vector>

#include "TCutG.h"
#include "TStyle.h"
#include "TTree.h"
#include "kobra.hxx"
TCutG* ssdpla_cut() {
    std::vector<Double_t> cutg_vect0{21.98295036312869, 18.09117862240883, 14.71830978045161, 10.75240905419422,
                                     8.083765574843454, 7.083024270086916, 8.973313401293709, 10.53002209758166,
                                     17.90585615856503, 26.57894746645502, 30.61897717824992, 34.43661993343227,
                                     34.02891051297591, 31.47146051193142, 27.46849529290527, 24.57746485694194,
                                     21.98295036312869};
    std::vector<Double_t> cutg_vect1{24.32862197262871, 20.32391048774773, 17.55594813790352, 14.31684326042626,
                                     11.66666654249032, 9.546525168141567, 7.956419137380002, 8.368668849058926,
                                     13.08009412538949, 20.67726738347252, 26.27208489911507, 31.57243833498695,
                                     33.45700844551918, 33.63368689338157, 29.98233230422539, 26.3309777150692,
                                     24.32862197262871};
    TCutG* cutg = new TCutG("CUTG", 17, cutg_vect0.data(), cutg_vect1.data());
    cutg->SetVarX("f3ssd.acal");
    cutg->SetVarY("f3dpla.GetAAveG()*1.6");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->Draw();
    return cutg;
}

void ssdpla() {
    KOBRA ko(KOBRA::Expt::Phys, std::vector<int>{569, 570, 571});
    TTree* tree = ko.GetTree();

    tree->SetAlias("plaa", "f3dpla.GetAAveG()*1.6");
    tree->SetAlias("ssda", "Sum$(f3ssd.acal)");

    TString eventcut = "plaa < 160 && ssda > 2 && aoq > 2 && aoq < 3.5";
    TCutG* cutg = ssdpla_cut();

    tree->Draw("plaa:ssda>>h2(200,0,40,200,0,40)", eventcut, "col");
    cutg->Draw("same");
}

void ssdpla_250924() {
    KOBRA ko(KOBRA::Expt::Phys, std::vector<int>{570, 571});
    ko.tree->Draw("Z_ssd3:aoq>>hpid_ssd(400,2,3,400,4,12)", "Length$(f3ssd.acal) > 0", "col");
    TH2* hpid_ssd = static_cast<TH2*>(gDirectory->Get("hpid_ssd"));
    hpid_ssd->SetTitle("PID with SSD;A/Q;Z");

    gStyle->SetOptStat(0);
    gPad->SetGrid(0, 0);
    gPad->SetGrid(0, 0);
    gPad->GetCanvas()->Print("figs/run570_pid_ssd.pdf");

    ko.tree->Draw("Z_pla:aoq>>hpid_pla(400,2,3,400,4,12)", "f3pla.GetTAve() > 0 && f3dpla.GetAAveG() > 0", "col");

    TH2* hpid_pla = static_cast<TH2*>(gDirectory->Get("hpid_pla"));
    hpid_pla->SetTitle("PID with Pla;A/Q;Z");

    gPad->GetCanvas()->Print("figs/run570_pid_pla.pdf");

    ko.tree->Draw("Max$(f3ssd.acal):f3dpla.GetAAveG()>>hplassd(400,0,50,400,0,50)",
                  "f3pla.GetTAve() > 0 && f3dpla.GetAAveG() > 0 && Length$(f3ssd.acal) > 0 && abs(f1uppacx.x) < 200",
                  "col");
    std::vector<Double_t> cutg_vect0{13.88799726702416, 7.87066726721774,  6.105583800607858, 4.581193533990233,
                                     5.54396633395926,  7.549743000561398, 11.4008342004375,  16.85654673359532,
                                     21.34948646678411, 22.15179713342497, 21.18902433345594, 17.81931953356435,
                                     14.04845940035233, 13.88799726702416};
    std::vector<Double_t> cutg_vect1{28.15884481241341, 17.32851974128169, 11.91335720571584, 7.739169417883818,
                                     7.062274100938085, 8.641696507144793, 14.73375435965639, 24.77436822768475,
                                     30.52797842172347, 34.02527089260976, 36.2815886157622,  35.15342975418598,
                                     29.85108310477774, 28.15884481241341};
    TCutG* cutg = new TCutG("CUTG", 14, cutg_vect0.data(), cutg_vect1.data());
    cutg->SetVarX("f3dpla.GetAAveG()");
    cutg->SetVarY("Max$(f3ssd.acal)");
    cutg->SetTitle("Graph");
    cutg->SetFillStyle(1000);
    cutg->SetLineColor(2);
    cutg->SetLineWidth(4);
    cutg->Draw();

    long long n = ko.tree->Draw("Z_ssd3:aoq>>hpid_ssd_cut(400,2,3,400,4,12)", "Length$(f3ssd.acal) > 0 && CUTG", "col");
    std::cout << "n: " << n << std::endl;
    TH2* hpid_ssd_cut = static_cast<TH2*>(gDirectory->Get("hpid_ssd_cut"));
    hpid_ssd_cut->SetTitle("PID with SSD (Cut)  ;A/Q;Z");

    gPad->GetCanvas()->Print("figs/run570_pid_ssd_cut.pdf");

    n = ko.tree->Draw("Z_pla:aoq>>hpid_pla_cut(400,2,3,400,4,12)",
                      "f3pla.GetTAve() > 0 && f3dpla.GetAAveG() > 0 && CUTG", "col");
    std::cout << "n: " << n << std::endl;
    TH2* hpid_pla_cut = static_cast<TH2*>(gDirectory->Get("hpid_pla_cut"));
    hpid_pla_cut->SetTitle("PID with Pla (Cut)  ;A/Q;Z");

    gPad->GetCanvas()->Print("figs/run570_pid_pla_cut.pdf");
}
