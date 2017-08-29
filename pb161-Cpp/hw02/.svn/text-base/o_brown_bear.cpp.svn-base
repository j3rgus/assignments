#include "o_brown_bear.h"

/********************TO DO******************/

Brown_bear::Brown_bear()
{
    m_size = adult_size();
    m_male = false;
}

Brown_bear::Brown_bear(bool male)
{
    m_size = adult_size();
    m_male = male;
}

void Brown_bear::feed()
{
    m_size *= 1.2;
}

Bear *Brown_bear::mate(Bear *hornyBear)
{
    if ((m_male == hornyBear->get_male()) || (m_size < adult_size()) || (hornyBear->get_size() < hornyBear->adult_size()))
        return NULL;
    Bear *newBear;
    if (m_size < hornyBear->get_size()) {
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

void Brown_bear::print_info() const
{
    std::cout << BB << ":" << get_size() << ":" << ((get_male()) ? MALE : FEMALE) << std::endl;
}

void Brown_bear::set_size(int size)
{
    m_size = size;
}

void Brown_bear::set_male(bool male)
{
    m_male = male;
}

/****************DO NOT EDIT****************/

Bear * Brown_bear::new_instance() const
{
    return new Brown_bear();
}

int Brown_bear::adult_size() const
{
    return 100;
}
