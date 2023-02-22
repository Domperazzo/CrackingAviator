/*
  c++ -o bestSaldo bestSaldo.cpp simulazione.cc `root-config --glibs --cflags`
*/

// =================================================================== //
//                                                                     //
// Author: Federico Venturoli                                          //
// Last edited: 10.08.2018                                             //
// Nella sezione delle simulazioni, il programma estrae M valori       //
// distribuiti secondo una poissoniana di cui si sono determinati      //
// i parametri precedentemente. Se la somma dei valori estratti supera //
// la soglia, viene aggiornata la variabile "saldo", In caso contrario,//
// la variabile "saldo" viene decrementata di "bet".                   //
// Alla fine delle N simulazioni, l programma cerca la combinazione    //
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

    TH1F *hist = new TH1F("hist", "Titolo istogramma", 800, 0, 40);

    for (auto val : data){
        hist->Fill(val);
    }

    TF1 fit_func("fit_func", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 0, 40);
    fit_func.SetParameters(50, 1, 0.3);
    hist->Fit("fit_func", "Q");
    fit_func.SetParName(0, "Lambda");

    std::cout << "Istogramma fittato" << "\n";

    double lambda = fit_func.GetParameter(0);

    

    int N = 10000; // Numero di simulazioni da eseguire
    int M = 100; // Numero di valori estratti in ogni simulazione

    
    int soglia_max = 10;
    int bet = 0;
    int bet_max = 100;
    double saldo1 = 100.0;
    double temp;
    vector<double> v_soglia;
    vector<double> v_bet;
    vector<double> v_saldi;

    TFile *file = new TFile("output.root", "RECREATE");
    TGraph2D *g = new TGraph2D();

    simulazione sim;

    sim.set_number_of_simulations(M);
    sim.set_lambda(lambda);
    int n = 0;
    do{
        //std::cout << "Entrato nel while" << "\n";
        std::vector<double> v_sim;
        
        for (int i = 0; i < soglia_max; i++){
           for (int j = 0; j < bet_max; j++){
               v_sim = sim.generator(fit_func);
               sim.set_moltiplicator(v_sim);
               sim.set_tresh(i);
               sim.set_saldo(saldo1);
               sim.set_bet(j);
               sim.do_simulation();
               g->SetPoint(i+j, sim.get_soglia(), sim.get_bet(), sim.get_saldo());
           }
           sim.set_saldo(saldo1);
           v_sim.clear();
        }
        n++;
        if (n % 200 == 0){
            std::cout<< "Simulazione numero:\t" << n << "\n";
        }
        
    } while (n < N);

    g->SetTitle("Andamento saldo in funzione di bet e soglia; soglia; bet; saldo");

    gStyle->SetPalette(1);
    TCanvas c1;
    g->SetMarkerStyle(20);
    g->Draw("pcol");
    g->Write("grafico2D");
    file->Close();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << '\n';

    theApp.Run();
    c1.Print("g1.pdf", "pdf");

    return 0;
    }