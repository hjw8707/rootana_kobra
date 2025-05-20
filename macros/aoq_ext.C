#ifndef __CLING__
#include <fstream>

#include "TCanvas.h"
#include "TH2.h"
#include "kobra.hxx"
#endif

void aoq_ext() {
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::o20, KOBRA::brhoValue["o20"]);
    TTree* tree = ko.tree;
    std::vector<Int_t> runNs = ko.GetRunNs();

    Int_t n = tree->Draw("aoq:scaler.ts:reft", "o20", "goff");

    Double_t* aoq = tree->GetV1();
    Double_t* ts = tree->GetV2();
    Double_t* reft = tree->GetV3();

    Double_t ts_ref = ts[0];
    Double_t reft_ref = reft[0];
    std::vector<Double_t> abs_time_s;  // 절대 시각을 저장할 벡터 (초 단위)

    for (Int_t i = 0; i < n; i++) {
        ts[i] = ts[i] - ts_ref;  // ns 단위의 상대 시간

        // 절대 시각 계산 (초 단위)
        Double_t s = ts[i] / 1e9 + reft[0];
        abs_time_s.push_back(s);
    }

    std::ofstream csv_file("aoq_reft.csv");
    csv_file << "reft,aoq\n";  // CSV 헤더 작성

    for (Int_t i = 0; i < n; i++) {
        if (aoq[i] > 0.5) {
            csv_file << reft[i] << "," << aoq[i] << "\n";  // aoq와 reft를 CSV로 저장
        }
    }

    csv_file.close();  // 파일 닫기
}