#include <iostream>
#include <fstream>
#include <cstdio>   // para printf
#include "../../include/player.h"
#include "inspect.h"

using namespace std;

void inspecionarBinario_Player(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if (!bin.is_open()) {
        cerr << "Erro ao abrir " << nomeArquivo << endl;
        return;
    }

    Player p;
    int i = 0;

    while (bin.read(reinterpret_cast<char*>(&p), sizeof(Player))) {
        cout << "Registro #" << ++i << endl;
        cout << "Bytes brutos: ";
        const unsigned char* dados = reinterpret_cast<unsigned char*>(&p);
        for (size_t j = 0; j < sizeof(Player); ++j) {
            printf("%02X ", dados[j]);
        }
        cout << endl;

        cout << "Interpretado:" << endl;
        cout << "ID: " << p.playerId << endl;
        cout << "Nickname: [" << p.nickname << "]" << endl;
        cout << "Country:  [" << p.country << "]" << endl;
        cout << "----------------------------" << endl;
    }

    bin.close();
}
