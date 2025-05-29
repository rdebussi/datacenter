#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "converter.h"
#include "include/purchased_games.h"

using namespace std;

// Função auxiliar para extrair os IDs dos jogos da string do CSV
vector<int> extractGameIds(const string& libraryStr) {
    vector<int> gameIds;
    string numStr;
    
    // Verifica se a string está vazia ou mal formada
    if (libraryStr.length() < 2) {
        cout << "String de biblioteca mal formada ou vazia: " << libraryStr << endl;
        return gameIds;
    }
    
    // Verifica se começa e termina com colchetes
    if (libraryStr.front() != '[' || libraryStr.back() != ']') {
        cout << "String de biblioteca mal formada (sem colchetes): " << libraryStr << endl;
        return gameIds;
    }
    
    // Remove os colchetes
    string content = libraryStr.substr(1, libraryStr.length() - 2);
    
    // Se o conteúdo estiver vazio após remover colchetes
    if (content.empty()) {
        return gameIds;
    }
    
    // Parse os números
    istringstream ss(content);
    while (getline(ss, numStr, ',')) {
        // Remove espaços em branco no início e fim
        numStr.erase(0, numStr.find_first_not_of(" \t\n\r"));
        numStr.erase(numStr.find_last_not_of(" \t\n\r") + 1);
        
        if (!numStr.empty()) {
            // Verifica se a string contém apenas dígitos (e opcionalmente um sinal negativo)
            bool isValidNumber = true;
            for (size_t i = 0; i < numStr.length(); i++) {
                if (!isdigit(numStr[i]) && !(i == 0 && numStr[i] == '-')) {
                    isValidNumber = false;
                    break;
                }
            }
            
            if (isValidNumber) {
                try {
                    int gameId = stoi(numStr);
                    gameIds.push_back(gameId);
                } catch (const exception& e) {
                    cout << "Erro ao converter ID: " << numStr << " - " << e.what() << endl;
                }
            } else {
                cout << "ID inválido encontrado: " << numStr << endl;
            }
        }
    }
    
    return gameIds;
}

void converterCSVparaBinario_PurchasedGames(const string& csvFilename, const string& binFilename) {
    ifstream csvFile(csvFilename);
    ofstream binFile(binFilename, ios::binary);
    
    if (!csvFile.is_open()) {
        cout << "Erro ao abrir arquivo CSV: " << csvFilename << endl;
        return;
    }
    if (!binFile.is_open()) {
        cout << "Erro ao criar arquivo binário: " << binFilename << endl;
        csvFile.close();
        return;
    }
    
    string line;
    int lineNumber = 0;
    
    // Pula o cabeçalho
    if (getline(csvFile, line)) {
        lineNumber++;
        cout << "Cabeçalho: " << line << endl;
    }
    
    while (getline(csvFile, line)) {
        lineNumber++;
        
        // Pula linhas vazias
        if (line.empty()) {
            continue;
        }
        
        // Remove possíveis caracteres de fim de linha
        line.erase(line.find_last_not_of("\r\n") + 1);

        PurchasedGames purchase;
        string playerId, libraryStr;
        
        // Procura a primeira vírgula que não está dentro de aspas
        bool insideQuotes = false;
        size_t pos = string::npos;
        
        for (size_t i = 0; i < line.length(); i++) {
            if (line[i] == '"') {
                insideQuotes = !insideQuotes;
            } else if (line[i] == ',' && !insideQuotes) {
                pos = i;
                break;
            }
        }
        
        if (pos != string::npos) {
            playerId = line.substr(0, pos);
            libraryStr = line.substr(pos + 1);
            
            // Remove aspas da biblioteca se existirem
            if (libraryStr.length() >= 2 && libraryStr.front() == '"' && libraryStr.back() == '"') {
                libraryStr = libraryStr.substr(1, libraryStr.length() - 2);
            }
            
            try {
                // Converte playerId para inteiro
                purchase.playerId = stoi(playerId);
                
                // Extrai os IDs dos jogos
                purchase.gameIds = extractGameIds(libraryStr);
                
                if (!purchase.gameIds.empty()) {
                    // Escreve o playerId
                    binFile.write(reinterpret_cast<char*>(&purchase.playerId), sizeof(int));
                    
                    // Escreve o tamanho do vetor
                    int size = purchase.gameIds.size();
                    binFile.write(reinterpret_cast<char*>(&size), sizeof(int));
                    
                    // Escreve os IDs dos jogos
                    binFile.write(reinterpret_cast<char*>(purchase.gameIds.data()), size * sizeof(int));
                    
                    cout << "Processado jogador " << purchase.playerId << " com " << size << " jogos (linha " << lineNumber << ")" << endl;
                } else {
                    cout << "Nenhum jogo válido encontrado para jogador " << purchase.playerId << " (linha " << lineNumber << ")" << endl;
                }
            } catch (const exception& e) {
                cout << "Erro ao processar linha " << lineNumber << ": " << line << endl;
                cout << "Erro: " << e.what() << endl;
            }
        } else {
            cout << "Formato inválido na linha " << lineNumber << ": " << line << endl;
        }
    }
    
    csvFile.close();
    binFile.close();
    
    cout << "Arquivo binário de jogos comprados gerado com sucesso!" << endl;
}