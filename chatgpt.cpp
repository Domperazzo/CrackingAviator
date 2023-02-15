

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<double> readData(string fileName)
{
    vector<double> data;
    // codice per leggere i dati dal file e inserirli in un vettore
    return data;
}

double mean(vector<double> data)
{
    // codice per calcolare la media dei dati
}

double covariance(vector<double> X, vector<double> Y)
{
    // codice per calcolare la covarianza tra i vettori X e Y
}

double variance(vector<double> data)
{
    // codice per calcolare la varianza dei dati
}

double calculateB1(vector<double> X, vector<double> Y)
{
    return covariance(X, Y) / variance(X);
}

double calculateB0(vector<double> X, vector<double> Y, double B1)
{
    double mean_X = mean(X);
    double mean_Y = mean(Y);
    return mean_Y - B1 * mean_X;
}

double predict(double B0, double B1, double X)
{
    return exp(B0 + B1 * X);
}

int main()
{
    vector<double> X = readData("X.txt");
    vector<double> Y = readData("Y.txt");

    double B1 = calculateB1(X, Y);
    double B0 = calculateB0(X, Y, B1);

    double X_test;
    cout << "Inserire valore di X per la previsione: ";
    cin >> X_test;
    cout << "La previsione per X=" << X_test << " è Y=" << predict(B0, B1, X_test) << endl;

    return 0;
}
