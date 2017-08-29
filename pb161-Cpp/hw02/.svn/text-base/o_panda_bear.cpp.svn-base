#include "o_panda_bear.h"

/********************TO DO******************/

Panda_bear::Panda_bear()
{
    m_size = adult_size();
    m_male = false;
}

Panda_bear::Panda_bear(bool male)
{
    m_size = adult_size();
    m_male = male;
}

void Panda_bear::feed()
{
    m_size *= 1.15;
}

Bear *Panda_bear::mate(Bear *hornyBear)
{
    if ((m_male == hornyBear->get_male()) || (m_size < adult_size()) || (hornyBear->get_size() < hornyBear->adult_size()))
        return NULL;
    Bear *newBear;
    if (m_size >= hornyBear->get_size()) {
        newBear = hornyBear->new_instance();
        newBear->set_male(hornyBear->get_male());
        newBear->set_size(hornyBear->adult_size()/2);
    } else {
        newBear = new_instance();
        newBear->set_male(m_male);
        newBear->set_size(adult_size()/2);
    }
    m_size = adult_size() / 2;
    hornyBear->set_size(hornyBear->adult_size() / 2);
    return newBear;
}

void Panda_bear::print_info() const
{
    std::cout << PANDA << ":" << get_size() << ":" << ((get_male()) ? MALE : FEMALE) << std::endl;
}

void Panda_bear::set_male(bool male)
{
    m_male = male;
}

void Panda_bear::set_size(int size)
{
    m_size = size;
}

/****************DO NOT EDIT****************/

Bear * Panda_bear::new_instance() const
{
    return new Panda_bear();
}

int Panda_bear::adult_size() const
{
    return 80;
}
