#ifndef PANDA_BEAR_H
#define PANDA_BEAR_H

#include "bear.h"

class Panda_bear : public Bear
{

public:
    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na default - false
     */
    Panda_bear();

    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na hodnotu parametra
     * @param pohlavie
     */
    Panda_bear(bool);

    virtual Bear * new_instance() const;
    virtual void feed();
    virtual Bear * mate(Bear *);
    virtual void print_info() const;
    virtual void set_size(int);
    virtual void set_male(bool);

    virtual int adult_size() const;

};

#endif // PANDA_BEAR_H
