#include <iostream>
#include <fstream>
#include <vector>
#include "btree.h"
#include "../include/purchased_games.h"

using namespace std;

void buildBTreeFromPurchasedGames(const string& binFile, BTree& btree) {
    cout << "Tentando abrir arquivo: " << binFile << endl;
    ifstream file(binFile, ios::binary);
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << binFile << endl;
        return;
    }
    cout << "Arquivo aberto com sucesso!" << endl;

    // Vai para o fim do arquivo para obter o tamanho
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);
    cout << "Tamanho do arquivo: " << fileSize << " bytes" << endl;

    int registro = 0;
    streampos lastPos = 0;
    
    while (file.tellg() < fileSize) {
        streampos pos = file.tellg();
        int playerId;
        int size;

        cout << "\nProcessando registro #" << registro << " na posição " << pos << endl;
        
        // Verifica se há espaço suficiente para ler o playerId e size
        if (pos + static_cast<streampos>(sizeof(int) * 2) > fileSize) {
            cout << "Fim do arquivo atingido (não há espaço para mais registros)" << endl;
            break;
        }

        // Tenta ler o ID do jogador
        if (!file.read(reinterpret_cast<char*>(&playerId), sizeof(int))) {
            cout << "Erro ao ler playerID no registro #" << registro << endl;
            break;
        }

        // Tenta ler o tamanho do vetor de jogos
        if (!file.read(reinterpret_cast<char*>(&size), sizeof(int))) {
            cout << "Erro ao ler size após playerID: " << playerId << endl;
            break;
        }

        cout << "PlayerID=" << playerId << ", Size=" << size << ", Offset=" << pos << endl;
        
        // Verifica se o tamanho é razoável
        if (size < 0 || size > 10000) {
            cout << "ERRO: Tamanho inválido (" << size << ") no registro #" << registro << endl;
            break;
        }

        // Verifica se há espaço suficiente para os gameIds
        if (pos + static_cast<streampos>(sizeof(int) * (2 + size)) > fileSize) {
            cout << "ERRO: Arquivo truncado no registro #" << registro << endl;
            break;
        }

        // Pula os IDs dos jogos
        file.seekg(size * sizeof(int), ios::cur);
        if (file.fail()) {
            cout << "ERRO: Falha ao pular gameIds no registro #" << registro << endl;
            break;
        }

        // Verifica se a posição atual faz sentido
        streampos currentPos = file.tellg();
        if (currentPos <= lastPos) {
            cout << "ERRO: Posição atual (" << currentPos << ") não avançou em relação à última posição (" << lastPos << ")" << endl;
            break;
        }
        lastPos = currentPos;

        cout << playerId << " -  " << pos << endl;
        btree.insert(playerId, pos);
        registro++;
        btree.search(playerId);

        // A cada 1000 registros, mostra progresso
        if (registro % 1000 == 0) {
            cout << "Processados " << registro << " registros..." << endl;
        }
    }

    cout << "\nTotal de registros processados: " << registro << endl;
    cout << "Total de entradas na btree: " << btree.entries.size() << endl;

    btree.build();
    file.close();

    // Mostra os primeiros 20 registros para verificação
    cout << "\nPrimeiros 20 registros da B-Tree ordenada:" << endl;
    int i = 0;
    for (const auto& entry : btree.entries) {
        if (i >= 20) break;
        cout << "Registro #" << i << ": PlayerID=" << entry.playerId << ", Offset=" << entry.offset << endl;
        i++;
    }
}

int main() {
    cout << "Iniciando construção da B-Tree..." << endl;
    BTree btree;
    buildBTreeFromPurchasedGames("bin/purchased_games.bin", btree);
    
    cout << "\nSalvando B-Tree no arquivo..." << endl;
    ofstream out("bin/btree.idx", ios::binary);
    if (!out.is_open()) {
        cerr << "Erro ao salvar a B-Tree." << endl;
        return 1;
    }

    int count = 0;
    for (const auto& entry : btree.entries) {
        out.write(reinterpret_cast<const char*>(&entry.playerId), sizeof(int));
        out.write(reinterpret_cast<const char*>(&entry.offset), sizeof(streampos));
        count++;
    }
    out.close();
    cout << "B-Tree construída e salva com sucesso. Total de entradas: " << count << endl;
    return 0;
}
