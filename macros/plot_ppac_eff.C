#ifndef __CLING__
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#endif

void plot_ppac_eff(const char* filename = "macros/ppac_eff_check.txt") {
  // 데이터 파싱
  std::map<std::string, std::map<std::string, double>> data;  // {setting: {ppac: efficiency}}
  std::vector<std::string> settings;
  std::vector<std::string> ppacs = {"f1uppacx", "f2dppac", "f3uppac", "f3dppac"};

  std::ifstream file(filename);
  std::string line;
  std::string current_setting;

  while (std::getline(file, line)) {
    // 세팅 라인 확인 (예: "o18 Runs")
    if (line.find("Runs") != std::string::npos) {
      current_setting = line.substr(0, line.find(" Runs"));
      if (std::find(settings.begin(), settings.end(), current_setting) == settings.end()) {
        settings.push_back(current_setting);
      }
    }
    // 데이터 라인 파싱 (예: "[cpp] r251 f1uppacx: Hit/All efficiency = 95.1589% (21563/22660)")
    else if (line.find("[cpp]") != std::string::npos && line.find("Hit/All efficiency") != std::string::npos) {
      size_t ppac_start = line.find("f");
      if (ppac_start == std::string::npos) continue;

      size_t ppac_end = line.find(":", ppac_start);
      if (ppac_end == std::string::npos) continue;

      std::string ppac = line.substr(ppac_start, ppac_end - ppac_start);

      size_t eff_start = line.find("=");
      if (eff_start == std::string::npos) continue;

      size_t eff_end = line.find("%", eff_start);
      if (eff_end == std::string::npos) continue;

      std::string eff_str = line.substr(eff_start + 1, eff_end - eff_start - 1);
      double efficiency = std::stod(eff_str);

      if (!current_setting.empty()) {
        data[current_setting][ppac] = efficiency;
      }
    }
  }

  file.close();

  // 세팅 순서 정의
  std::vector<std::string> ordered_settings = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};

  // Canvas 생성
  TCanvas* c1 = new TCanvas("c1", "PPAC Efficiency by Setting", 1600, 1000);
  c1->Divide(2, 2);

  // 색상 정의
  int colors[] = {kBlue, kRed, kGreen + 2, kMagenta + 2, kCyan + 2, kOrange + 2, kYellow + 2, kPink + 2};

  // 각 PPAC별로 그래프 생성
  for (int p = 0; p < 4; p++) {
    c1->cd(p + 1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.25);  // 하단 마진 증가로 X축 레이블 공간 확보
    gPad->SetTopMargin(0.1);      // 상단 마진 설정
    gPad->SetGrid();

    std::string ppac = ppacs[p];

    // 데이터 배열 준비
    int n_settings = ordered_settings.size();
    std::vector<double> x(n_settings);
    std::vector<double> y(n_settings);

    for (int i = 0; i < n_settings; i++) {
      x[i] = i + 0.5;
      std::string setting = ordered_settings[i];
      if (data.find(setting) != data.end() && data[setting].find(ppac) != data[setting].end()) {
        y[i] = data[setting][ppac];
      } else {
        y[i] = 0;
      }
    }

    // TH1F로 바 그래프 생성 (타이틀 제거)
    TH1F* h = new TH1F(Form("h_%s", ppac.c_str()), "", n_settings, 0, n_settings);
    h->SetFillColor(kBlue);
    h->SetFillStyle(1001);
    h->SetBarWidth(0.8);
    h->SetBarOffset(0.1);

    for (int i = 0; i < n_settings; i++) {
      h->SetBinContent(i + 1, y[i]);
    }

    h->SetMaximum(105);
    h->SetMinimum(0);
    h->GetXaxis()->SetTitle("Setting");
    h->GetYaxis()->SetTitle("Efficiency (%)");
    h->GetYaxis()->SetTitleOffset(1.2);

    // X축 레이블 설정 및 크기 조정
    for (int i = 0; i < n_settings; i++) {
      h->GetXaxis()->SetBinLabel(i + 1, ordered_settings[i].c_str());
    }

    h->GetXaxis()->LabelsOption("v");   // 세로로 레이블 표시
    h->GetXaxis()->SetLabelSize(0.08);  // X축 레이블 크기 증가
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.2);
    h->Draw("bar");

    // 제목 추가 (히스토그램 타이틀 대신 사용)
    TLatex* title = new TLatex();
    title->SetNDC();
    title->SetTextSize(0.08);
    title->SetTextAlign(22);
    title->SetTextFont(62);  // Bold font
    title->DrawLatex(0.5, 0.98, Form("%s", ppac.c_str()));

    // 각 바에 값 표시
    for (int i = 0; i < n_settings; i++) {
      if (y[i] > 0) {
        TLatex* label = new TLatex();
        label->SetTextSize(0.03);
        label->SetTextAlign(22);
        label->DrawLatex(x[i], y[i] + 2, Form("%.2f%%", y[i]));
      }
    }
  }

  c1->Update();
  c1->SaveAs("macros/ppac_eff_by_setting.png");
  c1->SaveAs("macros/ppac_eff_by_setting.pdf");

  std::cout << "Graph saved to macros/ppac_eff_by_setting.png and .pdf" << std::endl;
}
