#ifndef PRICE_H
#define PRICE_H

const int SIZE = 8;
const int DATE_SIZE = 11;

struct Price {
    int gameId;
    char usd[SIZE];
    char eur[SIZE];
    char gbp[SIZE];
    char jpy[SIZE];
    char rub[SIZE];
    char date_acquired[DATE_SIZE];
};

#endif