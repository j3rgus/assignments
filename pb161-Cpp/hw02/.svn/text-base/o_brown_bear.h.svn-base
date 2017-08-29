#ifndef BROWN_BEAR_H
#define BROWN_BEAR_H

#include "bear.h"

class Brown_bear : public Bear
{

public:
    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na default - false
     */
    Brown_bear();

    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na hodnotu parametra
     * @param pohlavie
     */
    Brown_bear(bool);

    virtual Bear * new_instance() const;
    virtual void feed();
    virtual Bear * mate(Bear *);
    virtual void print_info() const;
    virtual void set_size(int);
    virtual void set_male(bool);

    virtual int adult_size() const;

};

#endif // BROWN_BEAR_H
