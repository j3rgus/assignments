#include <cstdlib>
#include "o_zoo.h"

/********************TO DO******************/

Zoo::Zoo(int size)
{
    m_size = size;
    allocate_memory();
}

Zoo::Zoo(const Zoo &copy)
{
    m_size = copy.m_size;
    allocate_memory();
    for (int i = 0; i < copy.getNumBears(); i++) {
        add(copy.m_bear_cages[i]->new_instance());
        m_bear_cages[i]->set_male(copy.m_bear_cages[i]->get_male());
        m_bear_cages[i]->set_size(copy.m_bear_cages[i]->get_size());
    }
    for (int i = 0; i < copy.getNumSnails(); i++) {
        add(copy.m_snail_cages[i]->new_instance());
        m_snail_cages[i]->set_male(copy.m_snail_cages[i]->get_male());
        m_snail_cages[i]->set_size(copy.m_snail_cages[i]->get_size());
    }
}

Zoo::~Zoo()
{
    int allAnim;

    allAnim = getAllAnimals();
    for (int i = 0; i < allAnim; i++)
        deallocate_animal(m_all_animals[i]);
    deallocate_memory();
}

int compare(const void *a, const void *b)
{
    Animal *aa = *(Animal **)a;
    Animal *bb = *(Animal **)b;

    if (aa->get_size() == bb->get_size())
        return aa->get_id() - bb->get_id();
    else
        return aa->get_size() - bb->get_size();
}

void Zoo::print_cages()
{
    int allAnimals;

    allAnimals = getAllAnimals();
    qsort(&m_all_animals[0], allAnimals, sizeof(Animal *), compare);
    for (int i = 0; i < allAnimals; i++)
        m_all_animals[i]->print_info();
}

void Zoo::add(Bear *myBear)
{
    m_all_animals[getAllAnimals()] = myBear;
    m_bear_cages[getNumBears()] = myBear;
}

void Zoo::add(Snail *mySnail)
{
    m_all_animals[getAllAnimals()] = mySnail;
    m_snail_cages[getNumSnails()] = mySnail;
}

void Zoo::mate_season()
{
    Bear *newBear;
    Snail *newSnail;
    int numBears, numSnails;

    numBears = getNumBears();
    numSnails = getNumSnails();

    for (int i = 0; i < numSnails; i++) {
        newSnail = m_snail_cages[i]->mate();
        if (newSnail != NULL)
            add(newSnail);
    }
    for (int i = 0; i < numBears; i++) {
        for (int j = 0; j < numBears; j++) {
            if (i == j) continue;
            newBear = m_bear_cages[i]->mate(m_bear_cages[j]);
            if (newBear != NULL)
                add(newBear);
        }
    }
}

void Zoo::feed_all()
{
    int allAnimals;

    allAnimals = getAllAnimals();
    for (int i = 0; i < allAnimals; i++)
        m_all_animals[i]->feed();
}

int Zoo::getNumBears() const
{
    Bear **myBears = m_bear_cages;
    int len = 0;

    while (*myBears++ != NULL) {
        len++;
    }
    return len;
}

int Zoo::getNumSnails() const
{
    Snail **mySnails = m_snail_cages;
    int len = 0;

    while (*mySnails++ != NULL) {
        len++;
    }
    return len;
}

int Zoo::getAllAnimals() const
{
    Animal **myAnimals = m_all_animals;
    int len = 0;

    while (*myAnimals++ != NULL) {
        len++;
    }
    return len;
}

/****************DO NOT EDIT****************/
void Zoo::allocate_memory()
{
    m_bear_cages = new Bear *[m_size];
    m_snail_cages = new Snail *[m_size];
    m_all_animals = new Animal *[2*m_size];

    for(int i=0; i < m_size; i++)
    {
        m_bear_cages[i] = NULL;
        m_snail_cages[i] = NULL;
        m_all_animals[i] = NULL;
        m_all_animals[i+m_size] = NULL;
    }
}

void Zoo::deallocate_memory()
{
    delete[] m_bear_cages;
    delete[] m_snail_cages;
    delete[] m_all_animals;
}

void Zoo::deallocate_animal(Animal * a)
{
    delete a;
}

