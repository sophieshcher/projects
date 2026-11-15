/**
 * @file analyze_dimuon.cpp
 * @brief Reconstruction of the Z -> mu+ mu- invariant mass spectrum using CERN CMS Open Data.
 *
 * Supports both:
 *  1. Flat dimuon ROOT trees ("events" / "Events" with branches pt1, eta1, phi1, Q1, pt2, eta2, phi2, Q2)
 *  2. CMS NanoAOD outreach trees ("Events" with branches nMuon, Muon_pt, Muon_eta, Muon_phi, Muon_mass, Muon_charge)
 *
 * Performs a relativistic Breit-Wigner + exponential background fit around the Z-boson peak
 * (M_Z ~ 91.1876 GeV/c^2) and exports the publication-ready plot to results/z_boson_fit.png.
 */

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TTree.h"

constexpr double MUON_MASS_GEV = 0.1056583755; // GeV/c^2

/**
 * @brief Composite fit model: Relativistic Breit-Wigner signal + Exponential background.
 * par[0] = Signal normalization (N_sig)
 * par[1] = Resonance mass M_Z [GeV/c^2]
 * par[2] = Resonance width Gamma_Z [GeV]
 * par[3] = Background amplitude
 * par[4] = Background exponential slope
 */
double bw_plus_exp(const double* x, const double* par) {
    const double m = x[0];
    const double signal = par[0] * TMath::BreitWigner(m, par[1], par[2]);
    const double background = par[3] * std::exp(par[4] * (m - 90.0));
    return signal + background;
}

double exp_background(const double* x, const double* par) {
    const double m = x[0];
    return par[0] * std::exp(par[1] * (m - 90.0));
}

int main(int argc, char** argv) {
    std::string input_path = "data/Dimuon_DoubleMu.root";
    std::string output_plot = "results/z_boson_fit.png";

    if (argc > 1) {
        input_path = argv[1];
    }
    if (argc > 2) {
        output_plot = argv[2];
    }

    std::cout << "[INFO] Opening ROOT file: " << input_path << std::endl;
    TFile* file = TFile::Open(input_path.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "[ERROR] Could not open ROOT file: " << input_path << std::endl;
        std::cerr << "        Place Dimuon_DoubleMu.root or Run2012BC_DoubleMuParked_Muons.root in data/" << std::endl;
        return 1;
    }

    TTree* tree = dynamic_cast<TTree*>(file->Get("Events"));
    if (!tree) {
        tree = dynamic_cast<TTree*>(file->Get("events"));
    }
    if (!tree) {
        std::cerr << "[ERROR] Neither 'Events' nor 'events' TTree found in " << input_path << std::endl;
        file->Close();
        return 1;
    }

    // Histogram in the Z-boson invariant mass window [60, 120] GeV/c^2 (60 bins -> 1 GeV/bin)
    TH1D* h_mass = new TH1D(
        "h_dimuon_mass",
        ";m_{#mu^{+}#mu^{-}} [GeV/c^{2}];Events / 1.0 GeV",
        60, 60.0, 120.0
    );
    h_mass->Sumw2();

    const Long64_t n_entries = tree->GetEntries();
    std::cout << "[INFO] Total events in tree: " << n_entries << std::endl;

    // Detect branch layout: NanoAOD array layout vs flat dimuon layout
    if (tree->GetBranch("nMuon") && tree->GetBranch("Muon_pt")) {
        std::cout << "[INFO] Detected CMS NanoAOD branch structure (nMuon, Muon_pt, ...)" << std::endl;
        UInt_t nMuon = 0;
        Float_t Muon_pt[64], Muon_eta[64], Muon_phi[64], Muon_mass[64];
        Int_t Muon_charge[64];

        tree->SetBranchAddress("nMuon", &nMuon);
        tree->SetBranchAddress("Muon_pt", Muon_pt);
        tree->SetBranchAddress("Muon_eta", Muon_eta);
        tree->SetBranchAddress("Muon_phi", Muon_phi);
        tree->SetBranchAddress("Muon_mass", Muon_mass);
        tree->SetBranchAddress("Muon_charge", Muon_charge);

        TLorentzVector mu1, mu2;
        for (Long64_t i = 0; i < n_entries; ++i) {
            tree->GetEntry(i);
            if (nMuon < 2) continue;

            // Opposite-sign muon pair selection (mu+ mu-)
            if (Muon_charge[0] * Muon_charge[1] >= 0) continue;
            if (Muon_pt[0] < 10.0 || Muon_pt[1] < 10.0) continue;

            mu1.SetPtEtaPhiM(Muon_pt[0], Muon_eta[0], Muon_phi[0], Muon_mass[0]);
            mu2.SetPtEtaPhiM(Muon_pt[1], Muon_eta[1], Muon_phi[1], Muon_mass[1]);

            const double inv_mass = (mu1 + mu2).M();
            if (inv_mass >= 60.0 && inv_mass <= 120.0) {
                h_mass->Fill(inv_mass);
            }
        }
    } else if (tree->GetBranch("pt1") && tree->GetBranch("pt2")) {
        std::cout << "[INFO] Detected flat dimuon branch structure (pt1, eta1, phi1, pt2, ...)" << std::endl;
        Double_t pt1 = 0, eta1 = 0, phi1 = 0, pt2 = 0, eta2 = 0, phi2 = 0;
        Int_t q1 = 0, q2 = 0;

        tree->SetBranchAddress("pt1", &pt1);
        tree->SetBranchAddress("eta1", &eta1);
        tree->SetBranchAddress("phi1", &phi1);
        tree->SetBranchAddress("Q1", &q1);
        tree->SetBranchAddress("pt2", &pt2);
        tree->SetBranchAddress("eta2", &eta2);
        tree->SetBranchAddress("phi2", &phi2);
        tree->SetBranchAddress("Q2", &q2);

        TLorentzVector mu1, mu2;
        for (Long64_t i = 0; i < n_entries; ++i) {
            tree->GetEntry(i);
            if (q1 * q2 >= 0) continue;

            mu1.SetPtEtaPhiM(pt1, eta1, phi1, MUON_MASS_GEV);
            mu2.SetPtEtaPhiM(pt2, eta2, phi2, MUON_MASS_GEV);

            const double inv_mass = (mu1 + mu2).M();
            if (inv_mass >= 60.0 && inv_mass <= 120.0) {
                h_mass->Fill(inv_mass);
            }
        }
    } else {
        std::cerr << "[ERROR] Unrecognized branch structure. Run Events->Print() in ROOT." << std::endl;
        file->Close();
        return 1;
    }

    // Configure Breit-Wigner + Exponential Background Fit in [70, 110] GeV/c^2
    TF1* fit_func = new TF1("f_bw_exp", bw_plus_exp, 70.0, 110.0, 5);
    fit_func->SetParNames("N_{sig}", "M_{Z}", "#Gamma_{Z}", "B_{0}", "#lambda");
    fit_func->SetParameters(h_mass->GetMaximum() * 4.0, 91.1876, 2.4952, 5.0, -0.04);
    fit_func->SetParLimits(1, 88.0, 94.0);
    fit_func->SetParLimits(2, 1.5, 6.0);
    fit_func->SetLineColor(kRed + 1);
    fit_func->SetLineWidth(3);
    fit_func->SetNpx(500);

    std::cout << "[INFO] Performing Breit-Wigner + Background fit..." << std::endl;
    h_mass->Fit(fit_func, "RQM");

    // Extract background component for visualization
    TF1* bg_func = new TF1("f_bg", exp_background, 70.0, 110.0, 2);
    bg_func->SetParameters(fit_func->GetParameter(3), fit_func->GetParameter(4));
    bg_func->SetLineColor(kBlue + 1);
    bg_func->SetLineStyle(2);
    bg_func->SetLineWidth(2);

    // CMS publication plotting style
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    TCanvas* c = new TCanvas("c_z_boson", "CMS Open Data - Dimuon Z Boson Peak", 950, 700);
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.08);
    c->SetBottomMargin(0.12);

    h_mass->SetMarkerStyle(20);
    h_mass->SetMarkerSize(0.95);
    h_mass->SetMarkerColor(kBlack);
    h_mass->SetLineColor(kBlack);
    h_mass->SetMinimum(0.0);
    h_mass->Draw("E1P");

    bg_func->Draw("SAME");
    fit_func->Draw("SAME");

    // Legend & Fit Parameter Box
    TLegend* leg = new TLegend(0.56, 0.68, 0.92, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.033);
    leg->AddEntry(h_mass, "CMS Open Data (#mu^{+}#mu^{-})", "lep");
    leg->AddEntry(fit_func, "Breit-Wigner + Exp. Fit", "l");
    leg->AddEntry(bg_func, "Continuum Background", "l");
    leg->Draw();

    TLatex latex;
    latex.SetNDC();
    latex.SetTextFont(62);
    latex.SetTextSize(0.045);
    latex.DrawLatex(0.12, 0.93, "CMS Open Data");
    latex.SetTextFont(42);
    latex.SetTextSize(0.036);
    latex.DrawLatex(0.62, 0.93, "#sqrt{s} = 7-8 TeV, Z #rightarrow #mu^{+}#mu^{-}");

    latex.SetTextSize(0.032);
    latex.DrawLatex(0.56, 0.60, Form("M_{Z} = %.2f #pm %.2f GeV/c^{2}", fit_func->GetParameter(1), fit_func->GetParError(1)));
    latex.DrawLatex(0.56, 0.54, Form("#Gamma_{Z} = %.2f #pm %.2f GeV", fit_func->GetParameter(2), fit_func->GetParError(2)));
    latex.DrawLatex(0.56, 0.48, Form("#chi^{2} / ndf = %.1f / %d", fit_func->GetChisquare(), fit_func->GetNDF()));

    c->SaveAs(output_plot.c_str());
    std::cout << "[INFO] Saved invariant mass plot to: " << output_plot << std::endl;

    file->Close();
    return 0;
}
