#ifndef ZOO_H
#define ZOO_H

#include "o_white_bear.h"
#include "o_panda_bear.h"
#include "o_snail.h"

class Zoo
{
    /**
     * veľkosť zoo, teda počet klietok pre každý druh
     */
    int m_size;

    /**
     * dyn. alokované pole klietok pre medvede
     */
    Bear ** m_bear_cages;

    /**
     * dyn. alokované pole klietok pre slimáky
     */
    Snail** m_snail_cages;

    /**
     * dyn. alokované pole pre všetky zvieratá, obsahuje všetky zvieratá z klietok
     */
    Animal** m_all_animals;

public:
    /**
     * konštruktor inicializuje veľkosť zoo, dyn. alokuje pamať pre polia príslušnou metódou
     * @param veľkosť zoo
     */
    Zoo(int);

    Zoo(const Zoo &);

    Zoo & operator =(const Zoo &);

    /**
     * deštruktor, uvolní všetkú naalokovanú pamäť
     */
    ~Zoo();

    /**
     * vypíše info o každom zvierati v zoo, pomocou metódy print_info().
     * Zvieratá vypisuje v poradí podľa ich veľkosti vzostupne
     */
    void print_cages();

    /**
     * pridá medveďa do medvedích klietok a tiež do zoznamu všetkých zvierat
     * @param medveď
     */
    void add(Bear *);

    /**
     * pridá slimáka do klietok a tiež do zoznamu všetkých zvierat
     * @param slimák
     */
    void add(Snail *);

    /**
     * zavolá mate na všetkých zvieratách
     */
    void mate_season();

    /**
     * nakŕmi všetky zvieratá
     */
    void feed_all();

private:

    /**
     * dynamicky alokuje všetky 3 polia a nastaví ich na NULL;
     */
    void allocate_memory();

    /**
     * uvolní pamat teda polia alokované metódou allocate_memory, neuvolňuje pamat na ktorú ukazujú prvky polí
     */
    void deallocate_memory();

    /**
     * @param a uvolní pamat na ktorú ukazuje parameter a
     */
    void deallocate_animal(Animal * a);

    // funkcia vrati pocet medvedov
    int getNumBears() const;
    //funkcia vrati pocet slimakov
    int getNumSnails() const;
    //funkcia vrati pocet zvierat dokopy
    int getAllAnimals() const;

};

#endif // ZOO_H
