
#include <boost/thread/thread.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include "Semafor.h"


#include <iostream>
#include <string>
#include <set>

using namespace std;
using namespace boost;

minstd_rand generator(time(0));
uniform_int<> zakres(1, 3);

class Hangar;
class Samolot;

class Lotnisko {
public:

    Lotnisko(Hangar &h) : pasWolny(true), hangar(h), semaforPasa(1) {
    }
    void laduj(Samolot &s);
    void kolujDoHangaru(Samolot &s);
    void opuscHangar(Samolot &s);
    void startuj(Samolot &s);
private:
    bool pasWolny;
    Hangar &hangar;
    Semafor semaforPasa;
};

class Samolot {
public:

    Samolot(string n, Lotnisko &l) : nazwa(n), lotnisko(l) {
    }
    string nazwa;

    void operator()() {
        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        lotnisko.laduj(*this);

        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        lotnisko.kolujDoHangaru(*this);

        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        lotnisko.opuscHangar(*this);


        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        lotnisko.startuj(*this);
    }
private:
    Lotnisko &lotnisko;
};

class Hangar {
public:

    Hangar(int n) : rozmiarHangaru(n), semaforLiczbaSamolotow(n), semaforZasobow(1) {

    }

    void parkuj(Samolot &s) {
        semaforLiczbaSamolotow.czekaj();
        if (samoloty.size() < rozmiarHangaru) {
            samoloty.insert(s.nazwa);
        } else {
            cout << "Hangar pelny \n";
        }
        semaforLiczbaSamolotow.sygnalizuj();
    }

    void opusc(Samolot &s) {
        semaforZasobow.czekaj();
        if (samoloty.count(s.nazwa) == 1) {
            samoloty.erase(s.nazwa);
        } else {
            cout << "W hangarze nie ma tego samolotu \n";
        }
        semaforZasobow.sygnalizuj();
    }
private:
    unsigned rozmiarHangaru;
    set<string> samoloty;
    Semafor semaforLiczbaSamolotow;
    Semafor semaforZasobow;
};

void Lotnisko::laduj(Samolot &s) {
    semaforPasa.czekaj();
    if (pasWolny) {
        pasWolny = false;
        string info = "Samolot " + s.nazwa + " laduje \n";
        cout << info;
        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        info = "Samolot " + s.nazwa + " wyladowal \n";
        pasWolny = true;
    } else {
        cout << "Zderzenie na pasie startowym \n";
    }
    semaforPasa.sygnalizuj();
}

void Lotnisko::kolujDoHangaru(Samolot &s) {
    hangar.parkuj(s);
}

void Lotnisko::opuscHangar(Samolot &s) {
    hangar.opusc(s);
}

void Lotnisko::startuj(Samolot &s) {
    semaforPasa.czekaj();
    if (pasWolny) {
        pasWolny = false;
        string info = "Samolot " + s.nazwa + " startuje \n";
        cout << info;
        this_thread::sleep_for(chrono::seconds(zakres(generator)));
        info = "Samolot " + s.nazwa + " wystartowal \n";
        pasWolny = true;
    } else {
        cout << "Zderzenie na pasie startowym \n";
    }
    semaforPasa.sygnalizuj();

}
#define POJEMNOSC_HANGARU 5
#define LICZBA_SAMOLOTOW 10

int main(int argc, char** argv) {

    Hangar hangar(POJEMNOSC_HANGARU);
    Lotnisko lotnisko(hangar);
    thread_group watki;

    for (int i = 0; i < LICZBA_SAMOLOTOW; ++i) {
        stringstream napis;
        napis << "Rownolegle Linie Lotnicze" << i;
        watki.create_thread(Samolot(napis.str(), lotnisko));
    }

    watki.join_all();

    return 0;
}

