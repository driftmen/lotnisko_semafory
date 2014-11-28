#ifndef SEMAFOR_H_
#define SEMAFOR_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>


using namespace boost;

class Semafor
{
public:
Semafor(int _count);
void sygnalizuj();
void czekaj();
bool probuj_czekac();
int wartosc() const;

private:
int licznikZasobow;
mutex mutexLicznika;
condition_variable warunekLicznikRoznyOdZera;
};

#endif