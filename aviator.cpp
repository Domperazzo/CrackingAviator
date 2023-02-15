/*
  c++ -o main aviator.cpp  `root-config --glibs --cflags`
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TMath.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TAttFill.h"
#include "TMultiGraph.h"
#include "TStyle.h"

std::vector<double> readData(std::string fileName)
{
    std::ifstream file(fileName);
    std::vector<double> data;

    if (file.is_open())
    {
        double value;
        while (file >> value)
        {
            data.push_back(value);
        }
        file.close();
    }
    return data;
}

int main(int argc, char **argv)
{
    TApplication theApp("theApp", &argc, argv);
    gStyle->SetOptFit(1112);

    std::ifstream dati;
    dati.open("datiJoint.txt", std::ios::in);
    std::vector<double> v_dati;
    while (true)
    {
        double p0;
        dati >> p0;
        if (dati.eof() == true)
            break;
        v_dati.push_back(p0);
    }
    dati.close();

    TH1F h_distribuzione("h_distribuzione", "h_distribuzione", 800, 0, 40);

    for (int i = 0; i < v_dati.size(); i++){
        h_distribuzione.Fill(v_dati.at(i));
    }

    h_distribuzione.SetFillColor(kBlue + 1);
    h_distribuzione.GetXaxis()->SetTitle("valore estratto");
    h_distribuzione.GetYaxis()->SetTitle("conteggi per bin");

    int resolution_histogram = 6;

    TH1F h_freqDown("h_freqDown", "h_freqDown", 2*resolution_histogram, 0, resolution_histogram);
    TH1F h_freqUp("h_freqUp", "h_freqUp", 2*resolution_histogram, 0, resolution_histogram);
    double threshold = 1.80;
    int temp = 0;
    int countUp = 0;
    int countDown = 0;
    for (int i = 0; i < v_dati.size(); i++){
        if (v_dati.at(i) > threshold){
            h_freqUp.Fill(countUp);
            countUp = 0;
            } else{
            countUp++;
        }
        if (v_dati.at(i) < threshold){
            h_freqDown.Fill(countDown);
            // std::cout << count << "\n";
            countDown = 0;
        }
        else{
            countDown++;
        }
    }

    TGraph g1;
    for (int i = 0; i < v_dati.size(); i++){
        g1.SetPoint(i, i+1, v_dati.at(i));
    }
    

    TF1 *f1 = new TF1("f1", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 1, 40);
    TF1 *f2 = new TF1("f2", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 1, 40);
    TF1 *f3 = new TF1("f3", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 1, 40);
    f1->SetParameters(50, 1, 0.3);
    f2->SetParameters(50, 1, 0.3);
    f3->SetParameters(50, 1, 0.3);
    h_distribuzione.Fit("f1", "SQ");
    h_freqUp.Fit("f2", "SQ");
    h_freqDown.Fit("f3", "SQ");

    TCanvas c1;
    h_distribuzione.Draw();
    c1.Print("h_distribuzione.pdf", "pdf");

    TCanvas c2;
    h_freqUp.Draw();
    c2.Print("h_freqUp.pdf", "pdf");

    TCanvas c3;
    g1.Draw("ALP"); 
    c3.Print("g1.pdf", "pdf");

    TCanvas c4;
    h_freqDown.Draw();
    c4.Print("h_freqDown.pdf", "pdf");
    theApp.Run();

    return 0;
}
