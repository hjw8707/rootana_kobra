#ifndef __CLING__
#include "TCanvas.h"
#include "TEventList.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TTree.h"
#include "kobra.hxx"
#endif
void aoq_cor() {
  KOBRA ko(KOBRA::Expt::Phys, KOBRA::o20, KOBRA::brhoValue["o20"]);
  TTree* tree = ko.tree;
  // Select events for O-20, O-19, O-18
  tree->Draw(">>e_o20", "o20");
  tree->Draw(">>e_o19", "o19");
  tree->Draw(">>e_o18", "o18");

  TEventList* o20_list = static_cast<TEventList*>(gDirectory->Get("e_o20"));
  TEventList* o19_list = static_cast<TEventList*>(gDirectory->Get("e_o19"));
  TEventList* o18_list = static_cast<TEventList*>(gDirectory->Get("e_o18"));

  // Draw A/Q distributions and fit with Gaussian
  TCanvas* c1 = new TCanvas("c1", "A/Q Distributions", 1500, 500);
  c1->Divide(3, 1);

  // O-20: expected A/Q = 20/8 = 2.5
  c1->cd(1);
  tree->SetEventList(o20_list);
  tree->Draw("aoqcorp>>h_o20(100)");
  TH1* h_o20 = static_cast<TH1*>(gDirectory->Get("h_o20"));
  h_o20->SetTitle("O-20 A/Q Distribution");
  h_o20->Fit("gaus");
  TF1* f_o20 = h_o20->GetFunction("gaus");
  Double_t mean_o20 = f_o20->GetParameter(1);

  // O-19: expected A/Q = 19/8 = 2.375
  c1->cd(2);
  tree->SetEventList(o19_list);
  tree->Draw("aoqcorp>>h_o19(100)");
  TH1* h_o19 = static_cast<TH1*>(gDirectory->Get("h_o19"));
  h_o19->SetTitle("O-19 A/Q Distribution");
  h_o19->Fit("gaus");
  TF1* f_o19 = h_o19->GetFunction("gaus");
  Double_t mean_o19 = f_o19->GetParameter(1);

  // O-18: expected A/Q = 18/8 = 2.25
  c1->cd(3);
  tree->SetEventList(o18_list);
  tree->Draw("aoqcorp>>h_o18(100)");
  TH1* h_o18 = static_cast<TH1*>(gDirectory->Get("h_o18"));
  h_o18->SetTitle("O-18 A/Q Distribution");
  h_o18->Fit("gaus");
  TF1* f_o18 = h_o18->GetFunction("gaus");
  Double_t mean_o18 = f_o18->GetParameter(1);

  // Expected A/Q values
  Double_t expected_o20 = 20.0 / 8.0;  // 2.5
  Double_t expected_o19 = 19.0 / 8.0;  // 2.375
  Double_t expected_o18 = 18.0 / 8.0;  // 2.25

  // Fit 2nd order polynomial correction: aoq_cor = a0 + a1*aoq + a2*aoq^2
  // We need to solve: y = a0 + a1*x + a2*x^2 for 3 points
  // Using TGraph and polynomial fit
  TCanvas* c2 = new TCanvas("c2", "A/Q Correction", 800, 600);

  Double_t x[3] = {mean_o18, mean_o19, mean_o20};
  Double_t y[3] = {expected_o18, expected_o19, expected_o20};

  TGraph* gr = new TGraph(3, x, y);
  gr->SetTitle("A/Q Correction;Measured A/Q;Expected A/Q");
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.5);
  gr->Draw("AP");

  // Fit with 2nd order polynomial
  TF1* fpol2 = new TF1("fpol2", "pol2", 2.0, 2.8);
  gr->Fit(fpol2, "R");

  Double_t a0 = fpol2->GetParameter(0);
  Double_t a1 = fpol2->GetParameter(1);
  Double_t a2 = fpol2->GetParameter(2);

  // Print results
  std::cout << "========================================" << std::endl;
  std::cout << "A/Q Correction Coefficients (2nd order polynomial)" << std::endl;
  std::cout << "aoq_cor = a0 + a1*aoq + a2*aoq^2" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Measured values:" << std::endl;
  std::cout << "  O-18: " << mean_o18 << " (expected: " << expected_o18 << ")" << std::endl;
  std::cout << "  O-19: " << mean_o19 << " (expected: " << expected_o19 << ")" << std::endl;
  std::cout << "  O-20: " << mean_o20 << " (expected: " << expected_o20 << ")" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Correction coefficients:" << std::endl;
  std::cout << "  a0 = " << a0 << std::endl;
  std::cout << "  a1 = " << a1 << std::endl;
  std::cout << "  a2 = " << a2 << std::endl;
  std::cout << "========================================" << std::endl;

  // Reset event list
  tree->SetEventList(nullptr);
}