/*
  c++ -o bestSaldo bestSaldo.cpp simulazione.cc `root-config --glibs --cflags`
*/

// =================================================================== //
// Author: Federico Venturoli                                          //
//                                                                     //
// Last edited: 24.03.2023                                             //
//                                                                     //
// Nella sezione delle simulazioni, il programma estrae M valori       //
// distribuiti secondo una poissoniana di cui si sono determinati      //
// i parametri precedentemente. Se la somma dei valori estratti supera //
// la soglia, viene aggiornata la variabile "saldo", In caso contrario,//
// la variabile "saldo" viene decrementata di "bet".                   //
// Alla fine delle N simulazioni, il programma cerca la combinazione   //
// di "saldo" e "bet" che massimizza il saldo eseguendo un ciclo while //
// e for che confrontano il saldo ottenuto in ogni simulazione.        //
//                                                                     //
// Il programma assume che il file txt contenga solo una colonna di    //
// dati di tipo double e che i dati siano separati da spazi o da una   //
// nuova riga. Inoltre, il programma assume che il numero di dati nel  //
// file sia sufficientemente grande da poter effettuare un fit         //
// attendibile dell'istogramma. Se il numero di dati è troppo piccolo, //
// il fit potrebbe non essere preciso e il programma potrebbe          //
// restituire risultati errati.                                        //
//                                                                     //
// =================================================================== //

#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph2D.h"
#include "TApplication.h"
#include <chrono>
#include <ctime>

#include "simulazione.h"

using namespace std;

int main(int argc, char **argv){

    auto start = std::chrono::system_clock::now();

    // Leggi i dati dal file txt
    TApplication theApp("theApp", &argc, argv);
    gStyle->SetOptFit(1112);
    ifstream input_file("datiJoint.txt");
    vector<double> data;
    double value;
    while (input_file >> value){
        data.push_back(value);
    }
    input_file.close();

    TH1F *hist = new TH1F("hist", "Distribuzione moltiplicatore", 800, 1, 40);

    for (auto val : data){
        hist->Fill(val);
    }

    TF1 fit_func("fit_func", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 0, 40);
    fit_func.SetParameters(50, 1, 0.3);
    hist->Fit("fit_func", "Q");
    fit_func.SetParName(0, "Lambda");

    double lambda = fit_func.GetParameter(0);

    

    int N = 1000; // Numero di simulazioni da eseguire
    int M = 100; // Numero di valori estratti in ogni simulazione

    
    int soglia_max = 10;
    int bet = 0;
    int bet_max = 100;
    double saldo1 = 100.0;
    double temp;
    vector<double> v_soglia;
    vector<double> v_bet;
    vector<double> v_saldi;

    //TFile *file = new TFile("output.root", "RECREATE");

    TGraph2D *g = new TGraph2D();
    TH3F *h2 = new TH3F("hist3D", "hist3D", 10, -1000, 1000, 10, -1000, 1000, 10, -1000, 1000);
    simulazione sim;

    sim.set_number_of_simulations(M);

    /*
        Per la simulazione devo fare che imposto il numero di mani per simulazione e il numero totale
        di simulazioni. Fisso soglia e bet e imposto un saldo iniziale. Per ogni partita salvo il saldo
        finale in un vettore che sta nel private della classe
    */
   

    double const_tresh = 1.50;
    double const_bet = 1.00;
    double const_saldo = 100.0;
    int n_sim = 1000;

    sim.set_moltiplicator(sim.generator(fit_func));
    sim.set_tresh(const_tresh);
    sim.set_saldo(const_saldo);
    sim.set_bet(const_bet);
    sim.do_simulation();
    std::vector<double> v_saldo = sim.get_Vsaldo();
    /*
    h2->Fill(sim.get_soglia(), sim.get_bet(), sim.get_saldo());
    g->SetPoint(i + j, sim.get_soglia(), sim.get_bet(), sim.get_saldo());
            std::cout<< "Simulazione: " << n << "\t"
                     << "Soglia: " << i << "\t"
                     << "Bet: " << j << "\n";
    }
    */

    TH1F *hist2 = new TH1F("hist2", "Saldo", 800, 1, 40);
    for (int i = 0; i < v_saldo.size(); i++){
        hist2->Fill(v_saldo.at(i));
    }
    

    //g->SetTitle("Andamento saldo in funzione di bet e soglia; soglia; bet; saldo");

    gStyle->SetPalette(1);
    TCanvas c1;
    //g->SetMarkerStyle(20);
    //g->Draw("surf1");
    //TCanvas c2;
    hist2->Draw();
    //h2->Draw("pcol");
    //g->Write("grafico2D");
    //file->Close();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << '\n';

    //c1.Print("g1.pdf", "pdf");
    theApp.Run();

    return 0;
}