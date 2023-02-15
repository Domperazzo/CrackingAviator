/*
  c++ -o bestSaldo bestSaldo.cpp  `root-config --glibs --cflags`


  Nella sezione delle simulazioni, il programma estrae M valori distribuiti secondo la poissoniana con media lambda 
  utilizzando un generatore di numeri casuali TRandom3 di ROOT. 
  Se la somma dei valori estratti supera la soglia, viene aggiornata la variabile "saldo" come richiesto. 
  In caso contrario, la variabile "saldo" viene decrementata di "bet". Alla fine delle N simulazioni, 
  il programma cerca la combinazione di "saldo" e "bet" che massimizza il saldo eseguendo un ciclo for 
  che varia il valore di "bet" e confronta il saldo ottenuto in ogni simulazione.

  Il programma assume che il file txt contenga solo una colonna di dati di tipo double e che i 
  dati siano separati da spazi o da una nuova riga. Inoltre, il programma assume che il numero di dati nel file sia 
  sufficientemente grande da poter effettuare un fit attendibile dell'istogramma. Se il numero di dati è troppo piccolo, 
  il fit potrebbe non essere preciso e il programma potrebbe restituire risultati errati.
*/
#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TF1.h"
#include "TRandom3.h"

using namespace std;

int main()
{
    // Leggi i dati dal file txt
    ifstream input_file("datiJoint.txt");
    vector<double> data;
    double value;
    while (input_file >> value)
    {
        data.push_back(value);
    }
    input_file.close();

    TH1F *hist = new TH1F("hist", "Titolo istogramma", 100, 0, 10);

    for (auto val : data){
        hist->Fill(val);
    }

    TF1 *fit_func = new TF1("fit_func", "TMath::Poisson(x,[0])", 0, 10);
    fit_func->SetParameter(0, 1.0);
    hist->Fit("fit_func");
    fit_func->SetParName(0, "Lambda");

    double lambda = fit_func->GetParameter(0);

    // Crea un generatore di numeri casuali per la distribuzione poissoniana
    TRandom3 *rand = new TRandom3();

    int N = 10000; // Numero di simulazioni da eseguire
    int M = 50; // Numero di valori estratti in ogni simulazione

    
    double soglia = 1.5;
    double bet = 1.0;
    double saldo = 10.0;

    // Simulazioni
    for (int i = 0; i < N; i++){
        // Estrai M valori distribuiti secondo la poissoniana con lambda
        double sum = 0.0;
        for (int j = 0; j < M; j++){
            sum = rand->Poisson(lambda);
            // Se la somma supera la soglia, aggiorna il saldo
            if (sum > soglia){
                saldo = (saldo - bet) + bet * sum;
            }
            else{
                saldo = saldo - bet;
            }
        }
    }

    // Cerca la combinazione di saldo e bet che massimizza il saldo
    double max_saldo = 0.0;
    double best_bet = 0.0;
    for (double b = 0.1; b < 10.0; b += 0.1)
    {
        double s = 0.;
        for (int i = 0; i < N; i++)
        {
            double sum = 0.0;
            for (int j = 0; j < M; j++)
            {
                sum = rand->Poisson(lambda);
            }
            if (sum > soglia)
            {
                s = (s - b) + b * sum;
            }
            else
            {
                s = s - b;
            }
        }
        if (s > max_saldo)
        {
            max_saldo = s;
            best_bet = b;
        }
    }

    // Stampa il risultato
    cout << "Il saldo massimo è " << max_saldo << " con bet = " << best_bet << endl;

    return 0;
}