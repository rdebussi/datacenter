#include <iostream>
#include <fstream>
#include <vector>
#include "../include/player.h"
#include "../include/game.h"
#include "../include/purchased_games.h"
#include "btree.h"

using namespace std;

PurchasedGames readPurchasedGames(ifstream& file, streampos offset) {
    PurchasedGames pg;
    
    cout << "Posicionando no offset " << offset << endl;
    file.seekg(offset);
    if (file.fail()) {
        cout << "Erro ao posicionar no offset!" << endl;
        return pg;
    }

    // Lê o ID do jogador
    if (!file.read(reinterpret_cast<char*>(&pg.playerId), sizeof(int))) {
        cout << "Erro ao ler playerId!" << endl;
        return pg;
    }
    cout << "PlayerID lido: " << pg.playerId << endl;

    // Lê o tamanho do vetor de jogos
    int size;
    if (!file.read(reinterpret_cast<char*>(&size), sizeof(int))) {
        cout << "Erro ao ler size!" << endl;
        return pg;
    }
    cout << "Size lido: " << size << endl;

    // Verifica se o tamanho é razoável
    if (size < 0 || size > 10000) {  // assumindo que ninguém tem mais de 10000 jogos
        cout << "Tamanho inválido: " << size << endl;
        return pg;
    }

    // Lê os IDs dos jogos
    pg.gameIds.resize(size);
    if (!file.read(reinterpret_cast<char*>(pg.gameIds.data()), size * sizeof(int))) {
        cout << "Erro ao ler gameIds!" << endl;
        return pg;
    }

    cout << "Primeiros 5 gameIds (ou menos):" << endl;
    for (int i = 0; i < min(5, size); i++) {
        cout << pg.gameIds[i] << " ";
    }
    cout << endl;

    return pg;
}

Game readGameById(ifstream& gameFile, int id) {
    Game g;
    gameFile.clear();
    gameFile.seekg(0);
    while (gameFile.read(reinterpret_cast<char*>(&g), sizeof(Game))) {
        if (g.gameId == id) return g;
    }
    g.gameId = -1;
    return g;
}

int main() {
    cout << "Carregando índice B-Tree..." << endl;
    BTree btree;
    btree.loadFromFile("bin/btree.idx");
    cout << "Número de entradas na B-Tree: " << btree.entries.size() << endl;

    cout << "Abrindo arquivos de dados..." << endl;
    ifstream purchaseFile("bin/purchased_games.bin", ios::binary);
    ifstream gameFile("bin/games.bin", ios::binary);

    if (!purchaseFile.is_open() || !gameFile.is_open()) {
        cerr << "Erro ao abrir arquivos binários." << endl;
        return 1;
    }
    cout << "Arquivos abertos com sucesso!" << endl;

    int pid;
    cout << "Digite o ID do jogador: ";
    cin >> pid;
    cout << "Procurando jogador " << pid << " na B-Tree..." << endl;

    long offset = btree.search(pid);
    cout << "Resultado da busca - offset: " << offset << endl;
    
    if (offset == -1) {
        cout << "Jogador nao encontrado na base de dados." << endl;
        return 0;
    }

    cout << "Lendo dados do jogador no offset " << offset << "..." << endl;
    PurchasedGames pg = readPurchasedGames(purchaseFile, offset);

    cout << "==================================================================================" << endl;
    cout << "|        Jogador ID: " << pg.playerId << " possui " << pg.gameIds.size() << " jogos  |" << endl;
    cout << "==================================================================================" << endl;
    for (int gid : pg.gameIds) {
        Game g = readGameById(gameFile, gid);
        if (g.gameId != -1) {
            cout << "| " << g.title << " [" << g.platform << "]\n";
        }
    }
    cout << "==================================================================================" << endl;
    
    purchaseFile.close();
    gameFile.close();
    return 0;
}
