#ifndef __CLING__
#include "TBox.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLine.h"
#include "kobra.hxx"
#endif

void ssd_eff() {
  const char* isos[] = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};
  // const char* isos[] = {"o18"};
  std::map<std::string, std::pair<float, float>> aoq_cut = {
      {"o18", {2.36, 2.4}},  {"o19", {2.353, 2.392}}, {"o20", {2.495, 2.52}},  {"o21", {2.602, 2.636}},
      {"o22", {2.74, 2.78}}, {"ne24", {2.38, 2.415}}, {"ne25", {2.485, 2.52}}, {"ne26", {2.58, 2.62}},
  };

  TBox* box = new TBox(-25, -25, 25, 25);
  box->SetLineColor(kRed);
  box->SetLineWidth(2);
  box->SetFillStyle(0);  // no fill

  TCanvas* c1 = new TCanvas("c1", "SSD Efficiency", 1200, 900);
  c1->Divide(3, 3);
  int i = 1;
  for (const char* iso : isos) {
    c1->cd(i++);
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns[iso], KOBRA::brhoValue[iso]);
    ko.Draw(Form("f3uppac.y:f3uppac.x>>h%s(100,-50,50,100,-50,50)", iso),
            Form("aoq > %f && aoq < %f", aoq_cut[iso].first, aoq_cut[iso].second), "GOFF");
    TH2* h = (TH2*)gDirectory->Get(Form("h%s", iso));
    h->Draw("colz");
    box->Draw("same");

    ko.Draw("f3uppac.y:f3uppac.x>>htemp(100,-50,50,100,-50,50)",
            Form("aoq > %f && aoq < %f && f3uppac.y > -25 && f3uppac.y < 25 && f3uppac.x > -25 && f3uppac.x < 25",
                 aoq_cut[iso].first, aoq_cut[iso].second),
            "GOFF");
    TH2* htemp = (TH2*)gDirectory->Get("htemp");

    double ratio = htemp->GetEntries() / h->GetEntries();
    std::cout << iso << " Ratio of events in box to all events: " << ratio << std::endl;
    // INSERT_YOUR_CODE
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.06);
    latex.SetTextColor(kBlue + 2);
    latex.DrawLatex(0.15, 0.85, Form("%s: %.4f", iso, ratio));
  }
}

void ssd_xeff() {
  const char* isos[] = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};
  // const char* isos[] = {"o18"};
  std::map<std::string, std::pair<float, float>> aoq_cut = {
      {"o18", {2.36, 2.4}},  {"o19", {2.353, 2.392}}, {"o20", {2.495, 2.52}},  {"o21", {2.602, 2.636}},
      {"o22", {2.74, 2.78}}, {"ne24", {2.38, 2.415}}, {"ne25", {2.485, 2.52}}, {"ne26", {2.58, 2.62}},
  };

  TCanvas* c1 = new TCanvas("c1", "SSD X Efficiency", 1200, 900);
  c1->Divide(3, 3);
  int i = 1;
  for (const char* iso : isos) {
    c1->cd(i++);
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns[iso], KOBRA::brhoValue[iso]);
    ko.Draw(Form("f3uppac.x>>h%s(100,-50,50)", iso),
            Form("aoq > %f && aoq < %f", aoq_cut[iso].first, aoq_cut[iso].second), "GOFF");
    TH1* h = (TH1*)gDirectory->Get(Form("h%s", iso));
    h->Draw("hist");
    // INSERT_YOUR_CODE
    double y_min = h->GetMinimum();
    double y_max = h->GetMaximum();

    TLine* line_left = new TLine(-25, y_min, -25, y_max);
    line_left->SetLineColor(kRed);
    line_left->SetLineStyle(2);
    line_left->SetLineWidth(2);
    line_left->Draw("same");

    TLine* line_right = new TLine(25, y_min, 25, y_max);
    line_right->SetLineColor(kRed);
    line_right->SetLineStyle(2);
    line_right->SetLineWidth(2);
    line_right->Draw("same");

    ko.Draw("f3uppac.x>>htemp(100,-50,50)",
            Form("aoq > %f && aoq < %f && f3uppac.x > -25 && f3uppac.x < 25", aoq_cut[iso].first, aoq_cut[iso].second),
            "GOFF");
    TH1* htemp = (TH1*)gDirectory->Get("htemp");

    double ratio = htemp->GetEntries() / h->GetEntries();
    std::cout << iso << " Ratio of events in box to all events: " << ratio << std::endl;
    // INSERT_YOUR_CODE
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.06);
    latex.SetTextColor(kBlue + 2);
    latex.DrawLatex(0.15, 0.85, Form("%s: %.4f", iso, ratio));
  }
}

void ssd_yeff() {
  const char* isos[] = {"o18", "o19", "o20", "o21", "o22", "ne24", "ne25", "ne26"};
  // const char* isos[] = {"o18"};
  std::map<std::string, std::pair<float, float>> aoq_cut = {
      {"o18", {2.36, 2.4}},  {"o19", {2.353, 2.392}}, {"o20", {2.495, 2.52}},  {"o21", {2.602, 2.636}},
      {"o22", {2.74, 2.78}}, {"ne24", {2.38, 2.415}}, {"ne25", {2.485, 2.52}}, {"ne26", {2.58, 2.62}},
  };

  TCanvas* c1 = new TCanvas("c1", "SSD Y Efficiency", 1200, 900);
  c1->Divide(3, 3);
  int i = 1;
  for (const char* iso : isos) {
    c1->cd(i++);
    KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns[iso], KOBRA::brhoValue[iso]);
    ko.Draw(Form("f3uppac.y>>h%s(100,0,50)", iso),
            Form("aoq > %f && aoq < %f", aoq_cut[iso].first, aoq_cut[iso].second), "GOFF");
    TH1* h = (TH1*)gDirectory->Get(Form("h%s", iso));
    h->Draw("hist");
    // INSERT_YOUR_CODE
    double y_min = h->GetMinimum();
    double y_max = h->GetMaximum();

    TLine* line_right = new TLine(25, y_min, 25, y_max);
    line_right->SetLineColor(kRed);
    line_right->SetLineStyle(2);
    line_right->SetLineWidth(2);
    line_right->Draw("same");

    ko.Draw("f3uppac.y>>htemp(100,0,50)",
            Form("aoq > %f && aoq < %f && f3uppac.y > 0 && f3uppac.y < 25", aoq_cut[iso].first, aoq_cut[iso].second),
            "GOFF");
    TH1* htemp = (TH1*)gDirectory->Get("htemp");

    double ratio = htemp->Integral() / h->Integral();
    std::cout << iso << " Ratio of events in box to all events: " << ratio << std::endl;
    // INSERT_YOUR_CODE
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.06);
    latex.SetTextColor(kBlue + 2);
    latex.DrawLatex(0.15, 0.85, Form("%s: %.4f", iso, ratio));
  }
}