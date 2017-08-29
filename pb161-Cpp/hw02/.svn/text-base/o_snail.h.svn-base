#ifndef SNAIL_H
#define SNAIL_H
#include "animal.h"


class Snail : public Animal
{
public:

static const int ADULT = 50;

    /**
     * nastaví veľkosť slimáma na hodnotu dospelého a pohlavie na default - false
     */
    Snail();

    /**
     * nastaví veľkosť slimáka na hodnotu dospelého a pohlavie na hodnotu parametra
     * @param pohlavie
     */
    Snail(bool);

    /**
     * metóda nakrmi zviera, teda zväčší jeho veľkosť o nejaký zlomok jeho aktuálnej veľkosti
     */
    virtual void feed();

    /**
     * metóda množenia, množenie sa líši podľa druhu
     * @return vráti ukazatel na dynamicky alok. mláďa alebo NULL pri neuspechu
     */
    virtual Snail * mate();

    /**
     * vypíše na štandardný výstup informácie o konkrétnej inštancii - druh,
     * veľkosť a pohlavie oddelené znakom ':' a odriadkuje
     */
    virtual void print_info() const;

    /**
     * vráti ukazateľ na dynamicky alokovanú inštanciu prislušného druhu
     * novy objekt je inicializovany prislusnym bezparametrickym konstruktorom
     */
    virtual Snail * new_instance() const;

    /**
     * zmení veĺkosť jedinca na hodnotu parametru
     * @param nova veľkosť
     */
    virtual void set_size(int);

    /**
     * zmení pohlavie jedinca na hodnotu parametru
     * @param nové pohlavie
     */
    virtual void set_male(bool);

};

#endif // SNAIL_H
