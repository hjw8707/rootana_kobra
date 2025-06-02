#ifndef __CLING__
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TCutG.h"
#include "kobra.hxx"
#endif

void ppac_eff1() {
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // de-tof cut
    // 14C
    std::vector<Double_t> cutg_vect0{227.0025027862217, 220.1971209576547, 221.0575715336804, 225.5162699730864,
                                     233.72966183515,   237.9536919356399, 238.5794741727496, 235.2941174279241,
                                     231.0700873274342, 227.0025027862217};
    std::vector<Double_t> cutg_vect1{12.53191478233388, 11.21276582650681, 8.872340259716859, 8.744680683346498,
                                     9.468084949445212, 9.978723254926656, 11.72340413198826, 12.57446797445733,
                                     13.12765947206223, 12.53191478233388};
    TCutG *cutg = new TCutG("c14_cut", 10, cutg_vect0.data(), cutg_vect1.data());
    cutg->SetVarX("tof");
    cutg->SetVarY("de");
    cutg->SetTitle("14C");

    // 16N
    std::vector<Double_t> cutg_vect2{225.8291610916412, 219.4148931612677, 218.3197742463258, 219.571338720545,
                                     223.0913638042866, 235.2158946482854, 239.2834791894979, 240.2221525451623,
                                     239.0488108505818, 232.0869834627373, 228.488735599357,  225.8291610916412};
    std::vector<Double_t> cutg_vect3{16.53191484193853, 15.38297865460528, 13.85106373816094, 12.95744670356842,
                                     12.99999989569187, 13.8936169302844,  14.78723396487693, 16.10638292070399,
                                     17.68085102927178, 17.46808506865451, 16.78723399467925, 16.53191484193853};
    TCutG *cutg2 = new TCutG("n16_cut", 12, cutg_vect2.data(), cutg_vect3.data());
    cutg2->SetVarX("tof");
    cutg2->SetVarY("de");
    cutg2->SetTitle("16N");

    // 18O
    std::vector<Double_t> cutg_vect4{221.9962448893448, 218.1633286870484, 216.2859819757196, 222.9349182450092,
                                     231.2265328867116, 239.8310386469688, 238.8141425116657, 237.7972463763626,
                                     236.9367958003368, 232.2434290220147, 223.8735916006736, 221.9962448893448};
    std::vector<Double_t> cutg_vect5{20.61702128579008, 19.38297871420993, 17.85106379776559, 16.78723399467925,
                                     17.97872337413595, 19.17021275359266, 20.70212767003699, 21.72340428099988,
                                     22.27659577860478, 22.40425535497514, 21.3404255518888,  20.61702128579008};
    TCutG *cutg3 = new TCutG("o18_cut", 12, cutg_vect4.data(), cutg_vect5.data());
    cutg3->SetVarX("tof");
    cutg3->SetVarY("de");
    cutg3->SetTitle("18O");

    // 20Ne
    std::vector<Double_t> cutg_vect6{219.571338720545,  215.1126402811391, 215.5037541793326, 219.8060070594612,
                                     236.623904681782,  236.623904681782,  235.7634541057563, 232.2434290220147,
                                     225.1251560748929, 219.571338720545};
    std::vector<Double_t> cutg_vect7{25.25531922724654, 23.80851069504911, 21.3404255518888,  21.29787235976534,
                                     24.31914900053055, 25.51063837998726, 27.85106394677721, 27.63829798615994,
                                     26.36170222245633, 25.25531922724654};
    TCutG *cutg4 = new TCutG("f20_cut", 10, cutg_vect6.data(), cutg_vect7.data());
    cutg4->SetVarX("tof");
    cutg4->SetVarY("de");
    cutg4->SetTitle("20F");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<Int_t> runNs1 = {162, 163, 164, 165, 166, 167};
    std::vector<Int_t> runNs2 = {601, 602, 603, 604, 610, 611, 612, 615, 616, 617, 620, 621, 622, 623, 625};

    // KOBRA ko(KOBRA::Expt::Phys, runNs1, 1.2959);
    KOBRA ko(KOBRA::Expt::Phys, runNs2, 1.2959);

    std::map<std::string, std::vector<Double_t>> f1Eff;
    std::vector<Int_t> hvs = {565, 575, 585, 595, 600, 605};

    for (size_t i = 0; i < ko.GetRunNs().size(); ++i) {
        std::cout << "Analysis for run " << ko.GetRunNs()[i] << std::endl;
        f1Eff["14C"].push_back(ko.GetPPACEff(1, Form("c14_cut && runN==%d", ko.GetRunNs()[i]), false, "14C"));
        f1Eff["16N"].push_back(ko.GetPPACEff(1, Form("n16_cut && runN==%d", ko.GetRunNs()[i]), false, "16N"));
        f1Eff["18O"].push_back(ko.GetPPACEff(1, Form("o18_cut && runN==%d", ko.GetRunNs()[i]), false, "18O"));
        f1Eff["20F"].push_back(ko.GetPPACEff(1, Form("f20_cut && runN==%d", ko.GetRunNs()[i]), false, "20F"));
    }

    for (auto it = f1Eff.begin(); it != f1Eff.end(); ++it) {
        std::cout << it->first << ": " << it->second.size() << std::endl;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            size_t idx = it2 - it->second.begin();
            std::cout << hvs[idx] << " " << *it2 << std::endl;
        }
    }
}