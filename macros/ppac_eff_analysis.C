#ifndef __CLING__
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "TAxis.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TStyle.h"
#include "kobra.hxx"
#endif

// 데이터 생성 함수
void generate_ppac_eff_data_by_setting(const char* output_filename = "macros/ppac_eff_check.txt") {
  const char* isos[] = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};

  // 파일 출력 스트림 생성
  std::ofstream outfile(output_filename);
  if (!outfile.is_open()) {
    std::cerr << "Error: Cannot open file " << output_filename << std::endl;
    return;
  }

  std::string cut = "Max$(f3ssd@.GetEntriesFast()) > 0";
  for (const char* iso : isos) {
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns[iso], KOBRA::brhoValue[iso]);
    outfile << iso << " Runs" << std::endl;
    std::cout << iso << " Runs" << std::endl;
    ko.DrawPPACEff(cut.c_str(), outfile);
  }

  outfile.close();
  std::cout << "Data saved to " << output_filename << std::endl;
}

void generate_ppac_eff_data_by_Z(const char* output_filename = "macros/ppac_eff_check.txt") {
  int z[] = {6, 7, 8, 9, 10};
  const char* zs[] = {"c", "n", "o", "f", "ne"};

  // 파일 출력 스트림 생성
  std::ofstream outfile(output_filename);
  if (!outfile.is_open()) {
    std::cerr << "Error: Cannot open file " << output_filename << std::endl;
    return;
  }
  KOBRA ko(KOBRA::Expt::Phys, KOBRA::cs_all);
  for (int i = 0; i < 5; i++) {
    std::string cut = Form("abs(Z - %d) < 0.5", z[i]);
    outfile << zs[i] << " Element" << std::endl;
    std::cout << zs[i] << " Element" << std::endl;
    ko.DrawPPACEff(cut.c_str(), outfile);
  }

  outfile.close();
  std::cout << "Data saved to " << output_filename << std::endl;
}

// 그래프 그리기 함수
void plot_ppac_eff(const char* filename = "macros/ppac_eff_check.txt") {
  // 데이터 파싱
  std::map<std::string, std::map<std::string, double>> data;  // {setting: {ppac: efficiency}}
  std::vector<std::string> settings;
  std::vector<std::string> ppacs = {"f1uppacx", "f2dppac", "f3uppac", "f3dppac"};

  // PPAC 이름 매핑 (파일의 라벨 -> 내부 이름)
  std::map<std::string, std::string> ppac_map = {{"F1 Up PPACX Eff.", "f1uppacx"},
                                                 {"F2 Dn PPAC Eff.", "f2dppac"},
                                                 {"F3 Up PPAC Eff.", "f3uppac"},
                                                 {"F3 Dn PPAC Eff.", "f3dppac"}};

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
    // 데이터 라인 파싱 (예: " F1 Up PPACX Eff.: 0.937656")
    else if (line.find("Eff.") != std::string::npos && line.find(":") != std::string::npos) {
      // 라벨 부분 추출 (예: " F1 Up PPACX Eff.")
      size_t colon_pos = line.find(":");
      if (colon_pos == std::string::npos) continue;

      std::string label = line.substr(0, colon_pos);
      // 앞뒤 공백 제거
      while (!label.empty() && label[0] == ' ') label.erase(0, 1);
      while (!label.empty() && label[label.length() - 1] == ' ') label.erase(label.length() - 1);

      // PPAC 이름 매핑 확인
      if (ppac_map.find(label) == ppac_map.end()) continue;
      std::string ppac = ppac_map[label];

      // 효율 값 추출 (예: " 0.937656")
      std::string eff_str = line.substr(colon_pos + 1);
      // 공백 제거
      while (!eff_str.empty() && eff_str[0] == ' ') eff_str.erase(0, 1);
      while (!eff_str.empty() && eff_str[eff_str.length() - 1] == ' ') eff_str.erase(eff_str.length() - 1);

      double efficiency = std::stod(eff_str) * 100.0;  // 0.937656 -> 93.7656%

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

// 메인 함수: 데이터 생성 + 그래프 그리기
void ppac_eff_analysis() {
  const char* data_file = "macros/ppac_eff_by_setting.txt";

  std::cout << "=== Step 1: Generating PPAC efficiency data ===" << std::endl;
  generate_ppac_eff_data_by_setting(data_file);

  std::cout << "\n=== Step 2: Plotting PPAC efficiency graphs ===" << std::endl;
  plot_ppac_eff(data_file);

  std::cout << "\n=== Analysis complete! ===" << std::endl;
}
