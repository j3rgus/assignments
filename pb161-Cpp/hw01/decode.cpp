#include <iostream>
#include <cstring>
#include "decode.h"

using std::cerr;
using std::endl;

/* Zaujima nas len zaporna a kladna ampl. */
ParserManchester::ParserManchester(char positive, char negative, char zero)
{
    total = 0;
    if (positive == negative) {
        cerr << "Manchester initiation error" << endl;
        errorInit = true;
    } else {
        pos = positive;
        neg = negative;
        zer = zero;
        errorInit = false;
    }
}

bool ParserManchester::canParse(const char* input)
{
    char act, next;

    if (errorInit) return false;            //testujem spracnost vstupu
    if ((strlen(input) % 2)) return false;  //musi byt parnej dlzky
    while (*input) {                         //porovnaj kazych dvoch susedov (musia sa lisit)
       act =  *input++;
       next = *input++;
       if ( !((act==pos) && (next == neg)) && !((act == neg) && (next == pos)) )   //porovnavam "+-" a "-+"
           return false;
    }

    return true;
}

const char* ParserManchester::parse(const char* input)
{
    char sample[] = {pos, neg};
    int i = 0;

    while (*input) {
        if (!strncmp(input, sample, 2)) //predpokladam ze vstup je parsovatelny
            out[i] = '0';               //ak je "+-" zapis '0' inak '1'
        else out[i] = '1';
        input += 2;                     //posuniem sa v retazci dalej o dve miesta
        i++;                            //vo vystupe o jedno
    }
    out[i] = '\0';                      //vysledny retazec ukoncim nulou
    total++;
    return out;
}
/* Rovnaky princip ako pri Manchester */
ParserRZ::ParserRZ(char positive, char negative, char zero)
{
    total = 0;
    if (positive == zero) {
        cerr << "RZ initiation error" << endl;
        errorInit = true;
    } else {
        pos = positive;
        neg = negative;
        zer = zero;
        errorInit = false;
    }
}
/* Rovnaky princip ako pri Manchester, akurat porovnavam s inymi ampl. */
bool ParserRZ::canParse(const char* input)
{
    char act, next;

    if (errorInit) return false;
    if ((strlen(input) % 2)) return false;  //musi byt parnej dlzky
    while (*input) {
        act = *input++;
        next = *input++;
        if (!((act == pos) && (next == zer)) && !((act == zer) && (next == zer))) return false;
    }
    return true;
}
/* Rovanko ako pri Manchester */
const char* ParserRZ::parse(const char* input)
{
    char sample[] = {pos, zer};
    int i = 0;

    while (*input) {
        if (strncmp(input, sample, 2))
            out[i] = '0';
        else out[i] = '1';
        input += 2;
        i++;
    }
    out[i] = '\0';
    total++;
    return out;
}
/* Pracjuem len s kladnou a nulovou ampl. */
ParserNRZI::ParserNRZI(char positive, char negative, char zero)
{
    total = 0;
    if (positive == zero) {
        cerr << "NRZI initiation error" << endl;
        errorInit = true;
    } else {
        pos = positive;
        neg = negative;
        zer = zero;
        errorInit = false;
    }
}

bool ParserNRZI::canParse(const char* input)
{
    if (errorInit) return false;
    while (*input) {
        if ((*input != pos) && (*input != zer)) //pri vyskyte ineho znaku ako '+' a '0' vrat chybu
            return false;
        input++;
    }
    return true;
}

const char* ParserNRZI::parse(const char* input)
{
    char act, prev;
    int i = 0;

    while (*input) {
        prev = *input++;                //zapamatam si vzdy dva susedne znaky s tym
        act = *input;                   //ze nasledujuceho porovnam vzdy s predoslym
        if (act == prev) out[i] = '0';  // => ukazatel posuniem vzdy len o 1 tak aby som si pamatal posledneho
        else out[i] = '1';
        i++;
    }
    out[i-1] = '\0';            //na konci retazca porovnavam s nulou, preto tuto informaciu odstranim s vystupu
    total++;
    return out;
}
