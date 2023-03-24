#include <vector>
#include "TF1.h"
#include "TMath.h"
#include "TRandom3.h"

class simulazione{
public:
  
  simulazione();
  simulazione(int val1, int val2, int val3, int val4);
  ~simulazione();

  void set_number_of_simulations(int val);
  void set_tresh(double val);
  void set_bet(double val);
  void set_saldo(double val);
  void set_par1(double val);
  void set_par2(double val);
  void set_par3(double val);
  void set_lambda(double val);
  std::vector<double> generator(TF1 &dist_func); // ritorna il vettore dei moltiplicatori
  void set_moltiplicator(std::vector<double> val);
  void do_simulation();
  double get_saldo();
  double get_soglia();
  double get_bet();
  std::vector<double> get_Vsaldo();

      private : int N;
  double soglia;
  double bet;
  double saldo;
  double par1;
  double par2;
  double par3;
  std::vector<double> v_moltiplicator;
  std::vector<double> v_saldi;
  double lambda;
};
