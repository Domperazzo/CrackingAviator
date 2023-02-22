#include "simulazione.h"
#include <random>
#include "TF1.h"


simulazione::simulazione():
    N(0),
    soglia(0.0),
    bet(0.0), 
    saldo(0.0),
    par1(0.0),
    par2(0.0),
    par3(0.0)
{}
simulazione::simulazione(int val1, int val2, int val3, int val4):
    N(val1),
    soglia(val2),
    bet(val3),
    saldo(val4)
{}

simulazione::~simulazione()
{}

void simulazione::set_number_of_simulations(int val){
    N = val;
}

void simulazione::set_tresh(double val){
    soglia = val;
}

void simulazione::set_bet(double val){
    bet = val;
}

void simulazione::set_saldo(double val){
    saldo = val;
}

void simulazione::set_par1(double val){
    par1 = val;
}

void simulazione::set_par2(double val){
    par2 = val;
}

void simulazione::set_par3(double val){
    par3 = val;
}

void simulazione::set_lambda(double val){
    lambda = val;
}

void simulazione::set_moltiplicator(std::vector<double> val){
    v_moltiplicator = val;
}

double random(TF1 &func)
{
    std::mt19937 generator(std::random_device{}());

    double x_min = func.GetXmin();
    double x_max = func.GetXmax();

    while (true)
    {
        double x = std::uniform_real_distribution<double>(x_min, x_max)(generator);
        double y = func.Eval(x);
        double y_max = func.GetMaximum();
        double y_test = std::uniform_real_distribution<double>(0.0, y_max)(generator);
        if (y_test < y)
        {
            return x;
        }
    }
}

std::vector<double> simulazione::generator(TF1 &dist_func){
    std::vector<double> vec;
    for (int i = 0; i < N; i++){
        vec.push_back(random(dist_func));
    }
    return vec;
}

void simulazione::do_simulation(){
    for (int i = 0; i < N; i++){
        if (v_moltiplicator.at(i) > soglia){
            saldo = saldo - bet;
        } else{
            saldo = (saldo - bet) + bet * v_moltiplicator.at(i);
        }
    }
}

double simulazione::get_saldo(){
    return saldo;
}

double simulazione::get_soglia(){
    return soglia;
}
double simulazione::get_bet(){
    return bet;
}
