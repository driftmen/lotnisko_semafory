#include "Semafor.h"

Semafor::Semafor(int _count)
:licznikZasobow(_count){}

int Semafor::wartosc() const
{
return licznikZasobow;
}

void Semafor::sygnalizuj()
{
unique_lock<mutex> blokadaLicznika(mutexLicznika);
++licznikZasobow;
warunekLicznikRoznyOdZera.notify_one();
}

void Semafor::czekaj()
{
unique_lock<mutex> blokadaLicznika(mutexLicznika);
while(licznikZasobow==0)
warunekLicznikRoznyOdZera.wait(blokadaLicznika);
--licznikZasobow;
}

bool Semafor::probuj_czekac()
{
unique_lock<mutex> blokadaLicznika(mutexLicznika);
if(licznikZasobow!=0)
{
--licznikZasobow;
return true;
}
else return false;
}