/*
  c++ -o fitplot fit_plot.cpp `root-config --glibs --cflags`

*/
#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TApplication.h"
#include <chrono>
#include <ctime>
#include "TSpline3.h"
#include "TF3.h"

int main(int argc, char **argv){

    TFile *file = new TFile("output.root", "READ");

    TGraph2D *graph = (TGraph2D*)file->Get("grafico3D");
    graph->Draw("pcol");

    // Interpolazione della superficie di saldo
    TSpline3 *spline = new TSpline3("saldo_spline", graph);
    spline->Draw();
    int soglia_max = 10;
    int bet_max = 100;
    // Funzione di saldo interpolata
    TF3 *saldo_func = new TF3("saldo_func", "[0]", 0, bet_max, 0, soglia_max);
    saldo_func->SetParameter(0, 0);
    saldo_func->SetRange(0, 0, bet_max, soglia_max);
    saldo_func->SetNpx(100);
    saldo_func->SetNpy(100);
    saldo_func->SetNpz(100);
    saldo_func->SetFillColor(0);
    saldo_func->SetLineColor(kRed);
    saldo_func->SetLineWidth(2);
    saldo_func->Draw("same");

    // Ricerca dei massimi locali della funzione di saldo
    double max_saldo;
    double max_bet;
    double max_treshold;
    int n_max = 0;

    double delta_treshold = 0.1;
    double delta_bet = 0.1;

    for (double treshold = 0; treshold <= soglia_max; treshold += delta_treshold)
    {
        for (double bet = 0; bet <= bet_max; bet += delta_bet)
        {
            double saldo = saldo_func->GetMaximum(bet - delta_bet, treshold - delta_treshold, bet + delta_bet, treshold + delta_treshold, 1e-10, max_saldo);
            if (saldo == max_saldo)
            {
                max_bet = bet;
                max_treshold = treshold;
                n_max++;
                std::cout << "Maximo relativo in (" << max_bet << "," << max_treshold << "): " << max_saldo << "\n";
            }
        }
    }

    file->Close();

    return 0;
}