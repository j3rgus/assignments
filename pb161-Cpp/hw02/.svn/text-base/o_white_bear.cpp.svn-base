#include "o_white_bear.h"

/********************TO DO******************/

White_bear::White_bear()
{
    m_size = adult_size();
    m_male = false;
}

White_bear::White_bear(bool male)
{
    m_size = adult_size();
    m_male = male;
}

void White_bear::feed()
{
    m_size *= 1.3;
}

void White_bear::print_info() const
{
    std::cout << WB << ":" << get_size() << ":" << ((get_male()) ? MALE : FEMALE) << std::endl;
}

/****************DO NOT EDIT****************/

int White_bear::adult_size() const
{
    return 150;
}

Bear * White_bear::new_instance() const
{
    return new White_bear();
}
