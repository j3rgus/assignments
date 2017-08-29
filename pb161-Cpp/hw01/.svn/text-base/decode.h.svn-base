#ifndef DECODE_H
#define DECODE_H

class ParserManchester {
private:
    char pos;
    char neg;
    char zer;
    bool errorInit; //info o chybe
    int total;
    char out[50];   //sem ukladam vystupny retazec

public:
    ParserManchester():pos('+'),neg('-'),zer('0'),total(0){};
    ParserManchester(char positive, char negative, char zero);
    const char* parse(const char* input);
    bool canParse(const char* input);
    int getTotalParsed(){ return total; };
};

class ParserRZ {
private:
    char pos;
    char neg;
    char zer;
    bool errorInit; //info o chybe
    int total;
    char out[50];   //sem ukladam vystupny retazec

public:
    ParserRZ():pos('+'),neg('-'),zer('0'),total(0){};
    ParserRZ(char positive, char negative, char zero);
    const char* parse(const char* input);
    bool canParse(const char* input);
    int getTotalParsed(){ return total; };
};

class ParserNRZI {
private:
    char pos;
    char neg;
    char zer;
    bool errorInit;
    int total;
    char out[100];  //sem ukladam vystupny retazec

public:
    ParserNRZI():pos('+'),neg('-'),zer('0'),total(0){};
    ParserNRZI(char positive, char negative, char zero);
    const char* parse(const char* input);
    bool canParse(const char* input);
    int getTotalParsed(){ return total; };
};

#endif // DECODE_H
