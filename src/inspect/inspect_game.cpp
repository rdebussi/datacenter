
#include <iostream>
#include <fstream>
#include <cstdio>
#include "../../include/game.h"
#include "inspect.h"

using namespace std;

void InspecionarBinario_Game(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if(!bin.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    Game g;
    int i = 0;

    while(bin.read(reinterpret_cast<char*>(&g), sizeof(Game))){
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Registro #" << ++i << endl;
        cout << " -------------------------------------------------------------------------" << endl;
        cout << " | Game ID  : " << g.gameId << endl;
        cout << " | Title    : " << g.title << endl;
        cout << " | Platform : " << g.platform << endl;
        cout << " | Dev's    : " << g.developers << endl;
        cout << " | Publi's  : " << g.publishers << endl;
        cout << " | Genres   : " << g.genres << endl;
        cout << " | Langs    : " << g.supported_languages << endl;
        cout << " | Release  : " << g.release_date << endl;
        cout << " -------------------------------------------------------------------------" << endl;
    }

    bin.close();
}

