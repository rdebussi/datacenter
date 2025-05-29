#include <iostream>
#include <fstream>
#include <cstdio>
#include "../../include/achievement.h"
#include "inspect.h"

using namespace std;

void InspecionarBinario_Achievement(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if(!bin.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    Achievement a;
    int i = 0;

    while(bin.read(reinterpret_cast<char*>(&a), sizeof(Achievement))) {
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Registro #" << ++i << endl;
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Achievement ID    : " << a.achievementId << endl;
        cout << " | Game IDs         : ";
        for(int j = 0; j < GAME_SIZE && a.gameId[j] != 0; j++) {
            if(j > 0) cout << ", ";
            cout << a.gameId[j];
        }
        cout << endl;
        cout << " | Título           : " << a.title << endl;
        cout << " | Descrição        : " << a.description << endl;
        cout << " | Raridade         : " << a.rarity << endl;
        cout << " -------------------------------------------------------------------------" << endl;
    }

    bin.close();
} 