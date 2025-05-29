#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include "converter.h"
#include "../../include/achievement.h"
#include "../utils.h"

using namespace std;

// Função auxiliar para ler campo que pode conter vírgulas e quebras de linha entre aspas
string lerCampoCSV(ifstream& file, bool& erro) {
    string resultado;
    char c;
    bool dentroAspas = false;
    bool primeiroChar = true;
    
    // Verifica se o próximo caractere é uma vírgula ou nova linha (campo vazio)
    c = file.peek();
    if (c == ',' || c == '\n' || c == '\r') {
        if (c == ',') {
            file.get(); // Consome a vírgula
        }
        erro = false;
        return resultado; // Retorna string vazia para campo vazio
    }
    
    while (file.get(c)) {
        if (primeiroChar && c == '"') {
            dentroAspas = true;
            primeiroChar = false;
            continue;
        }
        
        if (dentroAspas) {
            if (c == '"') {
                if (file.peek() == '"') { // Aspas duplas escapadas
                    resultado += '"';
                    file.get(); // Consome a segunda aspa
                } else {
                    dentroAspas = false;
                }
            } else {
                resultado += c;
            }
        } else {
            if (c == ',' || c == '\n') {
                if (c == '\n') {
                    file.unget(); // Devolve o \n para o stream
                }
                break;
            } else if (c != '\r') {
                resultado += c;
            }
        }
        
        primeiroChar = false;
    }
    
    erro = dentroAspas; // Erro apenas se ainda estiver dentro de aspas
    return resultado;
}

void converterCSVparaBinario_Achievement(const string& csvPath, const string& binPath) {
    ifstream csvFile(csvPath, ios::binary);
    ofstream binWriter(binPath, ios::binary);

    if (!csvFile.is_open()) {
        cerr << "Erro ao abrir " << csvPath << endl;
        return;
    }
    if (!binWriter.is_open()) {
        cerr << "Erro ao criar " << binPath << endl;
        return;
    }

    string header;
    getline(csvFile, header); // pula o cabeçalho

    int lineNum = 1;
    bool erro = false;
    
    while (csvFile.good() && !csvFile.eof()) {
        lineNum++;
        
        // Verifica se a próxima linha está vazia
        char nextChar = csvFile.peek();
        if (nextChar == '\n' || nextChar == '\r') {
            csvFile.get(); // Consome o caractere
            if (nextChar == '\r' && csvFile.peek() == '\n') {
                csvFile.get(); // Consome o \n se for \r\n
            }
            continue; // Pula linha vazia
        }
        
        string achievementIdStr = lerCampoCSV(csvFile, erro);
        if (erro || csvFile.eof()) break;
        
        string gameIdStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler gameId na linha " << lineNum << endl;
            continue;
        }
        
        string titleStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler title na linha " << lineNum << endl;
            continue;
        }
        
        string descriptionStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler description na linha " << lineNum << endl;
            continue;
        }
        
        string rarityStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler rarity na linha " << lineNum << endl;
            continue;
        }

        // Remove espaços em branco
        achievementIdStr = trim(achievementIdStr);
        gameIdStr = trim(gameIdStr);
        titleStr = trim(titleStr);
        descriptionStr = trim(descriptionStr);
        rarityStr = trim(rarityStr);

        // Verifica apenas campos obrigatórios
        if (achievementIdStr.empty() || gameIdStr.empty() || titleStr.empty() || rarityStr.empty()) {
            cerr << "Erro: campo obrigatório vazio na linha " << lineNum << endl;
            cerr << "Conteúdo dos campos:" << endl;
            cerr << "achievementId: [" << achievementIdStr << "]" << endl;
            cerr << "gameId: [" << gameIdStr << "]" << endl;
            cerr << "title: [" << titleStr << "]" << endl;
            cerr << "description: [" << descriptionStr << "]" << endl;
            cerr << "rarity: [" << rarityStr << "]" << endl;
            continue;
        }

        // Preenche a estrutura
        Achievement a;
        
        strncpy(a.achievementId, achievementIdStr.c_str(), ACHIEVEMENT_SIZE);
        a.achievementId[ACHIEVEMENT_SIZE - 1] = '\0';
        
        // Inicializa o array de gameId com zeros
        for(int i = 0; i < GAME_SIZE; i++) {
            a.gameId[i] = 0;
        }

        // Converte o gameId
        try {
            a.gameId[0] = stoi(gameIdStr);
        } catch (const std::invalid_argument& e) {
            cerr << "Erro: ID de jogo inválido '" << gameIdStr << "' na linha " << lineNum << endl;
            continue;
        } catch (const std::out_of_range& e) {
            cerr << "Erro: ID de jogo fora do intervalo '" << gameIdStr << "' na linha " << lineNum << endl;
            continue;
        }
        
        strncpy(a.title, titleStr.c_str(), TITLE_SIZE);
        a.title[TITLE_SIZE - 1] = '\0';
        
        strncpy(a.description, descriptionStr.c_str(), DESCRIPTION_SIZE);
        a.description[DESCRIPTION_SIZE - 1] = '\0';
        
        strncpy(a.rarity, rarityStr.c_str(), RARITY_SIZE);
        a.rarity[RARITY_SIZE - 1] = '\0';

        // Escreve no arquivo binário
        binWriter.write(reinterpret_cast<char*>(&a), sizeof(Achievement));
    }

    csvFile.close();
    binWriter.close();
    cout << "Conversão de achievements concluída com sucesso para " << binPath << endl;
} 