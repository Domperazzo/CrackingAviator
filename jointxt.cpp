/*
  c++ -o jointxt jointxt.cpp  `root-config --glibs --cflags`
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

int main(int argc, char **argv){

    std::ifstream dati;
    dati.open("dati7.txt", std::ios::in);
    std::vector<double> v_dati;
    int line = 0;
    while (true)
    {
        double p0;
        dati >> p0;
        if (dati.eof() == true)
            break;
        v_dati.push_back(p0);
        line++;
    }
    dati.close();

    for (int i = 0; i < v_dati.size(); i++){
        std::ofstream outfile_media("datiJoint.txt", std::ios::app);
        if (outfile_media.is_open())
        {
            outfile_media << v_dati.at(i) << "\n";
            outfile_media.close();
        }
    }
    return 0;
}