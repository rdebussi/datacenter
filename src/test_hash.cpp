#include <iostream>
#include <fstream>
#include <limits>
#include <locale>
#include "../include/hash_table.h"

using namespace std;

// Declaração da função normalizeString do hash_table.cpp
void normalizeString(const char* input, char* output);

// Função para carregar jogadores do arquivo binário silenciosamente
void carregarJogadores(const string& filename, HashTableChaining& chainTable, HashTableOpen& openTable) {
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

void printStats(const HashStats& stats, const string& tableType) {
    cout << "\nEstatísticas da " << tableType << ":" << endl;
    cout << "Total de elementos: " << stats.totalElements << endl;
    cout << "Total de colisões: " << stats.totalCollisions << endl;
    cout << "Fator de carga: " << stats.loadFactor << endl;
    cout << "Tamanho máximo da cadeia: " << stats.maxChainLength << endl;
}

void inserirNovoJogador(HashTableChaining& chainTable, HashTableOpen& openTable) {
    Player novoJogador;
    
    cout << "\nInserção de novo jogador" << endl;
    cout << "Digite o ID do jogador: ";
    cin >> novoJogador.playerId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "Digite o nickname (max " << NICKNAME_SIZE-1 << " caracteres): ";
    cin.getline(novoJogador.nickname, NICKNAME_SIZE);
    
    cout << "Digite o país (max " << COUNTRY_SIZE-1 << " caracteres): ";
    cin.getline(novoJogador.country, COUNTRY_SIZE);
    
    // Inserir nas duas tabelas
    chainTable.insert(novoJogador);
    if (!openTable.insert(novoJogador)) {
        cout << "Aviso: Tabela com endereçamento aberto está cheia!" << endl;
    }
    
    cout << "\nJogador inserido com sucesso!" << endl;
}

void removerJogador(HashTableChaining& chainTable, HashTableOpen& openTable) {
    cout << "\nRemoção de jogador" << endl;
    cout << "Como deseja remover o jogador?" << endl;
    cout << "1. Por ID" << endl;
    cout << "2. Por nickname" << endl;
    cout << "Escolha uma opção: ";
    
    int opcao;
    cin >> opcao;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    bool removidoChain = false, removidoOpen = false;
    
    if (opcao == 1) {
        int playerId;
        cout << "Digite o ID do jogador: ";
        cin >> playerId;
        
        removidoChain = chainTable.remove(playerId);
        removidoOpen = openTable.remove(playerId);
    } else if (opcao == 2) {
        string nickname;
        cout << "Digite o nickname do jogador: ";
        getline(cin, nickname);
        
        removidoChain = chainTable.remove(nickname);
        removidoOpen = openTable.remove(nickname);
    } else {
        cout << "Opção inválida!" << endl;
        return;
    }
    
    if (removidoChain && removidoOpen) {
        cout << "Jogador removido com sucesso de ambas as tabelas!" << endl;
    } else if (removidoChain || removidoOpen) {
        cout << "Jogador removido parcialmente (apenas de uma tabela)!" << endl;
    } else {
        cout << "Jogador não encontrado!" << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    
    HashTableChaining chainTable;
    HashTableOpen openTable;

    // Carrega os jogadores silenciosamente
    carregarJogadores("bin/players.bin", chainTable, openTable);

    while (true) {
        cout << "\n=== Menu de Teste das Tabelas Hash ===" << endl;
        cout << "1. Buscar jogador por ID" << endl;
        cout << "2. Buscar jogador por nickname" << endl;
        cout << "3. Inserir novo jogador" << endl;
        cout << "4. Remover jogador" << endl;
        cout << "5. Mostrar estatísticas das tabelas" << endl;
        cout << "6. Sair" << endl;
        cout << "Escolha uma opção: ";

        int option;
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (option == 6) break;

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
            case 3:
                inserirNovoJogador(chainTable, openTable);
                break;
            case 4:
                removerJogador(chainTable, openTable);
                break;
            case 5: {
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