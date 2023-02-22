//
//  main.cpp
//  crackingAviator_XCodeVersion
//
//  Created by Federico Venturoli on 16/02/23.
//
//  c++ -o main main.cpp `root-config --glibs --cflags`
//

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

int main(int argc, char **argv) {
    
    TApplication theApp("theApp", &argc, argv);
    gStyle->SetOptFit(1112);
    std::ifstream input_file("datiJoint.txt");
    std::vector<double> v_data;
    double value;
    
    while (input_file >> value){
        v_data.push_back(value);
    }
    
    input_file.close();
    
    TH1F *hist = new TH1F("hist", "hist", 800, 0, 40);
    
    for (int i = 0; i<v_data.size(); i++){
        hist->Fill(v_data.at(i));
    }
    
    TF1 *f1 = new TF1("f1", "[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 1, 40);
    f1->SetParameters(50, 1, 0.3);
    hist->Fit("f1", "SQ");
    
    //TCanvas c1;
    //hist->Draw();
    //c1.Print("h_distribuzione.pdf", "pdf");
    
    
    
    return 0;
}
