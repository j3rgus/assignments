#ifndef WHITE_BEAR_H
#define WHITE_BEAR_H
#include "o_brown_bear.h"

class White_bear : public Brown_bear
{

public:
    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na default - false
     */
    White_bear();

    /**
     * nastaví veľkosť medveďa na hodnotu dospelého a pohlavie na hodnotu parametra
     * @param pohlavie
     */
    White_bear(bool);

    virtual void feed();
    virtual void print_info() const;

    virtual Bear * new_instance() const;
    virtual int adult_size() const;
};

#endif // WHITE_BEAR_H
