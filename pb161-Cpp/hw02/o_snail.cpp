#include "o_snail.h"

/********************TO DO******************/

Snail::Snail()
{
    m_size = ADULT;
    m_male = false;
}

Snail::Snail(bool male)
{
    m_size = ADULT;
    m_male = male;
}

void Snail::feed()
{
    m_size *= 1.1;
}

Snail *Snail::mate()
{
    if (m_size < ADULT)
        return NULL;
    Snail *newSnail = new_instance();
    m_size = ADULT/2;
    newSnail->set_size(ADULT/2);
    newSnail->set_male(!m_male);
    return newSnail;
}

void Snail::print_info() const
{
    std::cout << SN << ":" << get_size() << ":" << ((get_male()) ? MALE : FEMALE) << std::endl;
}

void Snail::set_male(bool male)
{
    m_male = male;
}

void Snail::set_size(int size)
{
    m_size = size;
}

/****************DO NOT EDIT****************/

Snail * Snail::new_instance() const
{
    return new Snail();
}
