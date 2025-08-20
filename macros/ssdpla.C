#include <vector>

#include "TCutG.h"
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