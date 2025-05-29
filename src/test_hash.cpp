#include <iostream>
#include <fstream>
#include "../include/hash_table.h"

using namespace std;

void printStats(const HashStats& stats, const string& tableType) {
    cout << "\nEstatísticas da " << tableType << ":" << endl;
    cout << "Total de elementos: " << stats.totalElements << endl;
    cout << "Total de colisões: " << stats.totalCollisions << endl;
    cout << "Fator de carga: " << stats.loadFactor << endl;
    cout << "Tamanho máximo da cadeia: " << stats.maxChainLength << endl;
}

void readAndPopulateTables(const string& filename, HashTableChaining& chainTable, HashTableOpen& openTable) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Erro ao abrir o arquivo " << filename << endl;
        return;
    }

    Player player;
    while (file.read(reinterpret_cast<char*>(&player), sizeof(Player))) {
        chainTable.insert(player);
        openTable.insert(player);
    }

    file.close();
}

int main() {
    HashTableChaining chainTable;
    HashTableOpen openTable;

    cout << "Carregando jogadores do arquivo binário..." << endl;
    readAndPopulateTables("bin/players.bin", chainTable, openTable);

    while (true) {
        cout << "\n=== Menu de Teste das Tabelas Hash ===" << endl;
        cout << "1. Buscar jogador por ID" << endl;
        cout << "2. Buscar jogador por nickname" << endl;
        cout << "3. Mostrar estatísticas das tabelas" << endl;
        cout << "4. Sair" << endl;
        cout << "Escolha uma opção: ";

        int option;
        cin >> option;

        if (option == 4) break;

        switch (option) {
            case 1: {
                cout << "Digite o ID do jogador: ";
                int playerId;
                cin >> playerId;

                cout << "\nResultados com Encadeamento:" << endl;
                Player* playerChain = chainTable.findByPlayerId(playerId);
                if (playerChain) {
                    cout << "Jogador encontrado:" << endl;
                    cout << "ID: " << playerChain->playerId << endl;
                    cout << "Nickname: " << playerChain->nickname << endl;
                    cout << "País: " << playerChain->country << endl;
                } else {
                    cout << "Jogador não encontrado." << endl;
                }

                cout << "\nResultados com Endereçamento Aberto:" << endl;
                Player* playerOpen = openTable.findByPlayerId(playerId);
                if (playerOpen) {
                    cout << "Jogador encontrado:" << endl;
                    cout << "ID: " << playerOpen->playerId << endl;
                    cout << "Nickname: " << playerOpen->nickname << endl;
                    cout << "País: " << playerOpen->country << endl;
                } else {
                    cout << "Jogador não encontrado." << endl;
                }
                break;
            }
            case 2: {
                cout << "Digite o nickname do jogador: ";
                string nickname;
                cin.ignore();
                getline(cin, nickname);

                cout << "\nResultados com Encadeamento:" << endl;
                Player* playerChain = chainTable.findByNickname(nickname);
                if (playerChain) {
                    cout << "Jogador encontrado:" << endl;
                    cout << "ID: " << playerChain->playerId << endl;
                    cout << "Nickname: " << playerChain->nickname << endl;
                    cout << "País: " << playerChain->country << endl;
                } else {
                    cout << "Jogador não encontrado." << endl;
                }

                cout << "\nResultados com Endereçamento Aberto:" << endl;
                Player* playerOpen = openTable.findByNickname(nickname);
                if (playerOpen) {
                    cout << "Jogador encontrado:" << endl;
                    cout << "ID: " << playerOpen->playerId << endl;
                    cout << "Nickname: " << playerOpen->nickname << endl;
                    cout << "País: " << playerOpen->country << endl;
                } else {
                    cout << "Jogador não encontrado." << endl;
                }
                break;
            }
            case 3: {
                HashStats statsChain = chainTable.getStats();
                HashStats statsOpen = openTable.getStats();

                printStats(statsChain, "Tabela Hash com Encadeamento");
                printStats(statsOpen, "Tabela Hash com Endereçamento Aberto");
                break;
            }
            default:
                cout << "Opção inválida!" << endl;
        }
    }

    return 0;
} 