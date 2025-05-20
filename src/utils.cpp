#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include "./utils.h"
#include "../include/player.h"

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == string::npos || last == string::npos) return "";
    return str.substr(first, (last - first + 1));
}

// void inspecionarBinario(const string& nomeArquivo) {
//     ifstream bin(nomeArquivo, ios::binary);
//     if (!bin.is_open()) {
//         cerr << "Erro ao abrir " << nomeArquivo << endl;
//         return;
//     }

//     Player p;
//     int i = 0;

//     while (bin.read(reinterpret_cast<char*>(&p), sizeof(Player))) {
//         cout << "Registro #" << ++i << endl;
//         cout << "Bytes brutos: ";
//         const unsigned char* dados = reinterpret_cast<unsigned char*>(&p);
//         for (size_t j = 0; j < sizeof(Player); ++j) {
//             printf("%02X ", dados[j]);
//         }
//         cout << endl;

//         cout << "Interpretado:" << endl;
//         cout << "ID: " << p.playerId << endl;
//         cout << "Nickname: [" << p.nickname << "]" << endl;
//         cout << "Country:  [" << p.country << "]" << endl;
//         cout << "----------------------------" << endl;
//     }

//     bin.close();
// }
