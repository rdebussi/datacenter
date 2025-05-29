#include <iostream>
#include <fstream>
#include <vector>
#include "inspect.h"
#include "include/purchased_games.h"

using namespace std;

void inspecionarBinario_PurchasedGames(const string& binFilename) {
    ifstream binFile(binFilename, ios::binary);
    
    if (!binFile.is_open()) {
        cout << "Erro ao abrir arquivo binário!" << endl;
        return;
    }
    
    while (true) {
        PurchasedGames purchase;
        int size;
        
        // Lê o playerId
        if (!binFile.read(reinterpret_cast<char*>(&purchase.playerId), sizeof(int))) {
            break;  // Fim do arquivo
        }
        
        // Lê o tamanho do vetor
        binFile.read(reinterpret_cast<char*>(&size), sizeof(int));
        
        // Lê os IDs dos jogos
        vector<int> gameIds(size);
        if (size > 0) {
            binFile.read(reinterpret_cast<char*>(gameIds.data()), size * sizeof(int));
        }
        
        // Exibe as informações
        cout << "\nJogador ID: " << purchase.playerId << endl;
        cout << "Quantidade de jogos: " << size << endl;
        cout << "Lista de jogos: [";
        for (int i = 0; i < size; i++) {
            cout << gameIds[i];
            if (i < size - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    
    binFile.close();
} 