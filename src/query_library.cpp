#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "../include/player.h"
#include "../include/game.h"
#include "../include/purchased_games.h"
#include "btree.h"

using namespace std;

Player readPlayerById(ifstream& playerFile, int id) {
    Player p;
    playerFile.clear();
    playerFile.seekg(0);
    while (playerFile.read(reinterpret_cast<char*>(&p), sizeof(Player))) {
        if (p.playerId == id) return p;
    }
    p.playerId = -1;
    return p;
}

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
    ifstream playerFile("bin/players.bin", ios::binary);

    if (!purchaseFile.is_open() || !gameFile.is_open() || !playerFile.is_open()) {
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

    // Lê os dados do jogador
    Player player = readPlayerById(playerFile, pid);
    if (player.playerId == -1) {
        cout << "Dados do jogador não encontrados." << endl;
        return 0;
    }

    // Lê a biblioteca de jogos
    PurchasedGames pg = readPurchasedGames(purchaseFile, offset);

    // Exibe o perfil do jogador
    cout << "==================================================================================" << endl;
    cout << "|                              PERFIL DO JOGADOR                                  |" << endl;
    cout << "==================================================================================" << endl;
    cout << "| Nickname: " << setw(67) << left << player.nickname << " |" << endl;
    cout << "| País: " << setw(71) << left << player.country << " |" << endl;
    cout << "| Total de jogos: " << setw(62) << left << pg.gameIds.size() << " |" << endl;
    cout << "==================================================================================" << endl;
    cout << "|                              BIBLIOTECA DE JOGOS                                |" << endl;
    cout << "==================================================================================" << endl;
    
    int jogosEncontrados = 0;
    for (int gid : pg.gameIds) {
        Game g = readGameById(gameFile, gid);
        if (g.gameId != -1) {
            cout << "| " << g.title << " [" << g.platform << "]\n";
            jogosEncontrados++;
        } else {
            cout << "| ID " << gid << " - Jogo não encontrado na base de dados\n";
        }
    }
    cout << "==================================================================================" << endl;
    cout << "Total de jogos encontrados: " << jogosEncontrados << " de " << pg.gameIds.size() << endl;
    
    purchaseFile.close();
    gameFile.close();
    playerFile.close();
    return 0;
}
