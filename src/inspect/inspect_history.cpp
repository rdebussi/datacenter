#include <iostream>
#include <fstream>
#include "../../include/history.h"
#include "inspect.h"

using namespace std;

void InspecionarBinario_History(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if (!bin.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    History h;
    int i = 0;

    while (bin.read(reinterpret_cast<char*>(&h), sizeof(History))) {
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Registro #" << ++i << endl;
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Player ID        : " << h.playerId << endl;
        cout << " | Achievement ID   : " << h.achievementId << endl;
        cout << " | Data de conquista: " << h.dateAcquired << endl;
        cout << " -------------------------------------------------------------------------" << endl;
    }

    bin.close();
}
