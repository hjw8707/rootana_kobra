#ifndef __CLING__
#include "TCanvas.h"
#include "TEventList.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TTree.h"
#include "kobra.hxx"
#endif
void z_cor() {
  KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns["o20"], KOBRA::brhoValue["o20"]);
  TTree* tree = ko.tree;
  tree->Draw("z_ssd3>>h_z_ssd3(800,0.1,0.6)");
  TH1* h_z_ssd3 = static_cast<TH1*>(gDirectory->Get("h_z_ssd3"));

  std::vector<std::pair<Double_t, Double_t>> z_ssd3_range = {{0.21, 0.23}, {0.245, 0.28}, {0.28, 0.33}, {0.33, 0.37},
                                                             {0.37, 0.42}, {0.42, 0.47},  {0.47, 0.5}};

  std::vector<Double_t> z_ssd3_par_list;
  for (const auto& range : z_ssd3_range) {
    h_z_ssd3->Fit("gaus", "Q0", "", range.first, range.second);
    TF1* f_z_ssd3 = h_z_ssd3->GetFunction("gaus");
    for (Int_t i = 0; i < 3; i++) {
      z_ssd3_par_list.push_back(f_z_ssd3->GetParameter(i));
    }
  }

  // Create a combined fit function: 7 Gaussians + 2nd order polynomial
  // Each Gaussian has 3 parameters: amplitude, mean, sigma
  // Polynomial has 3 parameters: p0, p1, p2
  // Total: 7*3 + 3 = 24 parameters
  TF1* f_combined = new TF1("f_combined",
                            "[0]*exp(-0.5*((x-[1])/[2])^2) + "
                            "[3]*exp(-0.5*((x-[4])/[5])^2) + "
                            "[6]*exp(-0.5*((x-[7])/[8])^2) + "
                            "[9]*exp(-0.5*((x-[10])/[11])^2) + "
                            "[12]*exp(-0.5*((x-[13])/[14])^2) + "
                            "[15]*exp(-0.5*((x-[16])/[17])^2) + "
                            "[18]*exp(-0.5*((x-[19])/[20])^2) + "
                            "[21] + [22]*x + [23]*x*x",
                            0.1, 0.6);
  f_combined->SetNpx(1000);

  // Set initial parameters for the 7 Gaussians from z_ssd3_par_list
  for (Int_t i = 0; i < 7; i++) {
    f_combined->SetParameter(i * 3, z_ssd3_par_list[i * 3]);          // amplitude
    f_combined->SetParameter(i * 3 + 1, z_ssd3_par_list[i * 3 + 1]);  // mean
    f_combined->SetParameter(i * 3 + 2, z_ssd3_par_list[i * 3 + 2]);  // sigma
  }

  // Set initial parameters for the 2nd order polynomial to 0
  f_combined->SetParameter(21, 0);  // p0
  f_combined->SetParameter(22, 0);  // p1
  f_combined->SetParameter(23, 0);  // p2

  // Perform the fit
  h_z_ssd3->Fit(f_combined, "R");

  // Print the results
  std::cout << "========================================" << std::endl;
  std::cout << "Combined Fit Results (7 Gaussians + Polynomial)" << std::endl;
  std::cout << "========================================" << std::endl;
  for (Int_t i = 0; i < 7; i++) {
    std::cout << "Gaussian " << i + 1 << ":" << std::endl;
    std::cout << "  Amplitude = " << f_combined->GetParameter(i * 3) << std::endl;
    std::cout << "  Mean      = " << f_combined->GetParameter(i * 3 + 1) << std::endl;
    std::cout << "  Sigma     = " << f_combined->GetParameter(i * 3 + 2) << std::endl;
  }
  std::cout << "Polynomial:" << std::endl;
  std::cout << "  p0 = " << f_combined->GetParameter(21) << std::endl;
  std::cout << "  p1 = " << f_combined->GetParameter(22) << std::endl;
  std::cout << "  p2 = " << f_combined->GetParameter(23) << std::endl;
  std::cout << "========================================" << std::endl;

  // Extract the 7 Gaussian means and find the correction polynomial
  // We want to map the 5 middle means to Z = 6, 7, 8, 9, 10
  std::vector<Double_t> means;
  for (Int_t i = 0; i < 7; i++) {
    means.push_back(f_combined->GetParameter(i * 3 + 1));
  }

  // Sort the means
  std::sort(means.begin(), means.end());

  // Remove the smallest and largest means (keep indices 1-5)
  std::vector<Double_t> selected_means;
  for (Int_t i = 1; i <= 5; i++) {
    selected_means.push_back(means[i]);
  }

  // Expected Z values: 6, 7, 8, 9, 10
  Double_t expected_z[5] = {6.0, 7.0, 8.0, 9.0, 10.0};

  // Create a graph for fitting
  TGraph* gr_z_cor = new TGraph(5);
  for (Int_t i = 0; i < 5; i++) {
    gr_z_cor->SetPoint(i, selected_means[i], expected_z[i]);
  }

  // Fit with 2nd order polynomial
  TF1* fpol2_z = new TF1("fpol2_z", "pol2", selected_means[0] - 0.1, selected_means[4] + 0.1);
  gr_z_cor->Fit(fpol2_z, "R");

  Double_t z_a0 = fpol2_z->GetParameter(0);
  Double_t z_a1 = fpol2_z->GetParameter(1);
  Double_t z_a2 = fpol2_z->GetParameter(2);

  // Print Z correction results
  std::cout << "========================================" << std::endl;
  std::cout << "Z Correction Polynomial (2nd order)" << std::endl;
  std::cout << "Z_cor = a0 + a1*z + a2*z^2" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Selected means (sorted, excluding min and max):" << std::endl;
  for (Int_t i = 0; i < 5; i++) {
    std::cout << "  z = " << selected_means[i] << " -> Z = " << expected_z[i] << std::endl;
  }
  std::cout << "========================================" << std::endl;
  std::cout << "Correction coefficients:" << std::endl;
  std::cout << "  a0 = " << z_a0 << std::endl;
  std::cout << "  a1 = " << z_a1 << std::endl;
  std::cout << "  a2 = " << z_a2 << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "SetAlias format:" << std::endl;
  std::cout << Form("  tree->SetAlias(\"Z_ssd3\", \"%f+%f*z_ssd3+%f*z_ssd3*z_ssd3\");", z_a0, z_a1, z_a2) << std::endl;
  std::cout << "========================================" << std::endl;

  // Draw the correction graph
  TCanvas* c_z_cor = new TCanvas("c_z_cor", "Z Correction", 800, 600);
  gr_z_cor->SetTitle("Z Correction;Measured z;Expected Z");
  gr_z_cor->SetMarkerStyle(20);
  gr_z_cor->SetMarkerSize(1.5);
  gr_z_cor->Draw("AP");
  fpol2_z->Draw("same");
}

void z_cor_pla_cs() {
  KOBRA ko(KOBRA::Expt::Phys, KOBRA::csruns["o20"], KOBRA::brhoValue["o20"]);
  ko.Draw("z_pla>>h_z_pla(800,0.08,0.18)", "f3ssd@.GetEntriesFast() > 0 && (o20 || n17 || f22 || c15 || ne24)", "col");

  TH1* h_z_pla = static_cast<TH1*>(gDirectory->Get("h_z_pla"));

  std::vector<std::pair<Double_t, Double_t>> z_pla_range = {
      {0.1, 0.11}, {0.113, 0.124}, {0.124, 0.134}, {0.135, 0.15}, {0.154, 0.17}};

  std::vector<Double_t> z_pla_par_list;
  for (const auto& range : z_pla_range) {
    h_z_pla->Fit("gaus", "Q0", "", range.first, range.second);
    TF1* f_z_pla = h_z_pla->GetFunction("gaus");
    for (Int_t i = 0; i < 3; i++) {
      z_pla_par_list.push_back(f_z_pla->GetParameter(i));
    }
  }

  // Create a combined fit function: 5 Gaussians + 2nd order polynomial
  // Each Gaussian has 3 parameters: amplitude, mean, sigma
  // Polynomial has 3 parameters: p0, p1, p2
  // Total: 5*3 + 3 = 18 parameters
  TF1* f_combined = new TF1("f_combined",
                            "[0]*exp(-0.5*((x-[1])/[2])^2) + "
                            "[3]*exp(-0.5*((x-[4])/[5])^2) + "
                            "[6]*exp(-0.5*((x-[7])/[8])^2) + "
                            "[9]*exp(-0.5*((x-[10])/[11])^2) + "
                            "[12]*exp(-0.5*((x-[13])/[14])^2) + "
                            "[15] + [16]*x + [17]*x*x",
                            0.08, 0.18);
  f_combined->SetNpx(1000);

  // Set initial parameters for the 7 Gaussians from z_ssd3_par_list
  for (Int_t i = 0; i < 5; i++) {
    f_combined->SetParameter(i * 3, z_pla_par_list[i * 3]);          // amplitude
    f_combined->SetParameter(i * 3 + 1, z_pla_par_list[i * 3 + 1]);  // mean
    f_combined->SetParameter(i * 3 + 2, z_pla_par_list[i * 3 + 2]);  // sigma
  }

  // Set initial parameters for the 2nd order polynomial to 0
  f_combined->SetParameter(15, 0);  // p0
  f_combined->SetParameter(16, 0);  // p1
  f_combined->SetParameter(17, 0);  // p2

  // Perform the fit
  h_z_pla->Fit(f_combined, "R");

  // Print the results
  std::cout << "========================================" << std::endl;
  std::cout << "Combined Fit Results (5 Gaussians + Polynomial)" << std::endl;
  std::cout << "========================================" << std::endl;
  for (Int_t i = 0; i < 5; i++) {
    std::cout << "Gaussian " << i + 1 << ":" << std::endl;
    std::cout << "  Amplitude = " << f_combined->GetParameter(i * 3) << std::endl;
    std::cout << "  Mean      = " << f_combined->GetParameter(i * 3 + 1) << std::endl;
    std::cout << "  Sigma     = " << f_combined->GetParameter(i * 3 + 2) << std::endl;
  }
  std::cout << "Polynomial:" << std::endl;
  std::cout << "  p0 = " << f_combined->GetParameter(15) << std::endl;
  std::cout << "  p1 = " << f_combined->GetParameter(16) << std::endl;
  std::cout << "  p2 = " << f_combined->GetParameter(17) << std::endl;
  std::cout << "========================================" << std::endl;

  // Extract the 5 Gaussian means and find the correction polynomial
  // We want to map the 5 middle means to Z = 6, 7, 8, 9, 10
  std::vector<Double_t> means;
  for (Int_t i = 0; i < 5; i++) {
    means.push_back(f_combined->GetParameter(i * 3 + 1));
  }

  // Sort the means
  std::sort(means.begin(), means.end());

  // Remove the smallest and largest means (keep indices 1-5)
  std::vector<Double_t> selected_means;
  for (Int_t i = 0; i < 5; i++) {
    selected_means.push_back(means[i]);
  }

  // Expected Z values: 6, 7, 8, 9, 10
  Double_t expected_z[5] = {6.0, 7.0, 8.0, 9.0, 10.0};

  // Create a graph for fitting
  TGraph* gr_z_cor = new TGraph(5);
  for (Int_t i = 0; i < 5; i++) {
    gr_z_cor->SetPoint(i, selected_means[i], expected_z[i]);
  }

  // Fit with 2nd order polynomial
  TF1* fpol2_z = new TF1("fpol2_z", "pol2", selected_means[0] - 0.1, selected_means[4] + 0.1);
  gr_z_cor->Fit(fpol2_z, "R");

  Double_t z_a0 = fpol2_z->GetParameter(0);
  Double_t z_a1 = fpol2_z->GetParameter(1);
  Double_t z_a2 = fpol2_z->GetParameter(2);

  // Print Z correction results
  std::cout << "========================================" << std::endl;
  std::cout << "Z Correction Polynomial (2nd order)" << std::endl;
  std::cout << "Z_cor = a0 + a1*z + a2*z^2" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Selected means (sorted, excluding min and max):" << std::endl;
  for (Int_t i = 0; i < 5; i++) {
    std::cout << "  z = " << selected_means[i] << " -> Z = " << expected_z[i] << std::endl;
  }
  std::cout << "========================================" << std::endl;
  std::cout << "Correction coefficients:" << std::endl;
  std::cout << "  a0 = " << z_a0 << std::endl;
  std::cout << "  a1 = " << z_a1 << std::endl;
  std::cout << "  a2 = " << z_a2 << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "SetAlias format:" << std::endl;
  std::cout << Form("  tree->SetAlias(\"Z_pla\", \"%f+%f*z_pla+%f*z_pla*z_pla\");", z_a0, z_a1, z_a2) << std::endl;
  std::cout << "========================================" << std::endl;

  // Draw the correction graph
  TCanvas* c_z_cor = new TCanvas("c_z_cor", "Z Correction", 800, 600);
  gr_z_cor->SetTitle("Z Correction;Measured z;Expected Z");
  gr_z_cor->SetMarkerStyle(20);
  gr_z_cor->SetMarkerSize(1.5);
  gr_z_cor->Draw("AP");
  fpol2_z->Draw("same");
}