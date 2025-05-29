#include <iostream>
#include <fstream>
#include <vector>
#include "btree.h"
#include "../include/purchased_games.h"

using namespace std;

void buildBTreeFromPurchasedGames(const string& binFile, BTree& btree) {
    ifstream file(binFile, ios::binary);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << binFile << endl;
        return;
    }

    // Vai para o fim do arquivo para obter o tamanho
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int registro = 0;
    streampos lastPos = 0;
    
    cout << "Construindo B-Tree..." << endl;
    
    while (file.tellg() < fileSize) {
        streampos pos = file.tellg();
        int playerId;
        int size;

        // Verifica se há espaço suficiente para ler o playerId e size
        if (pos + static_cast<streampos>(sizeof(int) * 2) > fileSize) {
            break;
        }

        if (!file.read(reinterpret_cast<char*>(&playerId), sizeof(int)) ||
            !file.read(reinterpret_cast<char*>(&size), sizeof(int))) {
            break;
        }

        // Verifica se o tamanho é razoável
        if (size < 0 || size > 10000) {
            cerr << "ERRO: Tamanho inválido (" << size << ") no registro #" << registro << endl;
            break;
        }

        // Verifica se há espaço suficiente para os gameIds
        if (pos + static_cast<streampos>(sizeof(int) * (2 + size)) > fileSize) {
            cerr << "ERRO: Arquivo truncado no registro #" << registro << endl;
            break;
        }

        // Pula os IDs dos jogos
        file.seekg(size * sizeof(int), ios::cur);
        if (file.fail()) {
            cerr << "ERRO: Falha ao pular gameIds no registro #" << registro << endl;
            break;
        }

        btree.insert(playerId, pos);
        registro++;

        // Mostra progresso a cada 1000 registros
        if (registro % 1000 == 0) {
            cout << "Processados " << registro << " registros\r" << flush;
        }
    }

    cout << "\nTotal de registros processados: " << registro << endl;
    
    file.close();
}

int main() {
    BTree btree;
    buildBTreeFromPurchasedGames("bin/purchased_games.bin", btree);
    
    cout << "Salvando índice..." << endl;
    ofstream out("bin/btree.idx", ios::binary);
    if (!out.is_open()) {
        cerr << "Erro ao salvar a B-Tree." << endl;
        return 1;
    }

    for (const auto& entry : btree.entries) {
        out.write(reinterpret_cast<const char*>(&entry.playerId), sizeof(int));
        out.write(reinterpret_cast<const char*>(&entry.offset), sizeof(streampos));
    }
    out.close();
    
    cout << "B-Tree construída e salva com sucesso. Total de entradas: " << btree.size() << endl;
    return 0;
}
