#include <iostream>
#include <fstream>
#include <cstdio>
#include "../../include/price.h"
#include "inspect.h"

using namespace std;

void InspecionarBinario_Price(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if(!bin.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    Price p;
    int i = 0;

    while(bin.read(reinterpret_cast<char*>(&p), sizeof(Price))) {
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Registro #" << ++i << endl;
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Game ID           : " << p.gameId << endl;
        cout << " | Preço USD        : " << p.usd << endl;
        cout << " | Preço EUR        : " << p.eur << endl;
        cout << " | Preço GBP        : " << p.gbp << endl;
        cout << " | Preço JPY        : " << p.jpy << endl;
        cout << " | Preço RUB        : " << p.rub << endl;
        cout << " | Data Aquisição   : " << p.date_acquired << endl;
        cout << " -------------------------------------------------------------------------" << endl;
    }

    bin.close();
} 