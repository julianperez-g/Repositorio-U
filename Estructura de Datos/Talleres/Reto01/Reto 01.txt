Reto 01 :

#include <iostream>
using namespace std;

class Cafetera {
  public:
      virtual bool Capuchino() = 0;
      virtual bool Tinto() = 0;
};

class Oster : public Cafetera {
  public:
    bool Capuchino() override {
        cout << "Preparando Capuchino" << endl;
        return true;
    }
    bool Tinto() override {
        cout << "Preparando Tinto" << endl;
        return true;
    }
};

class Haceb : public Cafetera {
  public:
    bool Capuchino() override {
        cout << "Preparando Capuchino" << endl;
        return true;
    }
    bool Tinto() override {
        cout << "Preparando Tinto" << endl;
        return true;
    }
};

int main() {
  Cafetera* Cafetera_Oster = new Oster();
  Cafetera_Oster->Capuchino();
  Cafetera_Oster->Tinto();

  Cafetera* Cafetera_Haceb = new Haceb();
  Cafetera_Haceb->Capuchino();
  Cafetera_Haceb->Tinto();

  delete Cafetera_Oster;
  delete Cafetera_Haceb;

  return 0;
}
