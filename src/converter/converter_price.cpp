#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "converter.h"
#include "../../include/price.h"
#include "../utils.h"

using namespace std;

void converterCSVparaBinario_Price(const string& csvPath, const string& binPath) {
    ifstream csvFile(csvPath);
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

    string line;
    while (getline(csvFile, line)) {
        stringstream ss(line);
        string idStr, usdStr, eurStr, gbpStr, jpyStr, rubStr, dateStr;
        
        // Lê os campos separados por vírgula
        getline(ss, idStr, ',');
        getline(ss, usdStr, ',');
        getline(ss, eurStr, ',');
        getline(ss, gbpStr, ',');
        getline(ss, jpyStr, ',');
        getline(ss, rubStr, ',');
        getline(ss, dateStr, ',');

        // Remove espaços em branco
        idStr = trim(idStr);
        usdStr = trim(usdStr);
        eurStr = trim(eurStr);
        gbpStr = trim(gbpStr);
        jpyStr = trim(jpyStr);
        rubStr = trim(rubStr);
        dateStr = trim(dateStr);

        // Preenche a estrutura
        Price p;
        p.gameId = stoi(idStr);
        
        strncpy(p.usd, usdStr.c_str(), SIZE);
        p.usd[SIZE - 1] = '\0';
        
        strncpy(p.eur, eurStr.c_str(), SIZE);
        p.eur[SIZE - 1] = '\0';
        
        strncpy(p.gbp, gbpStr.c_str(), SIZE);
        p.gbp[SIZE - 1] = '\0';
        
        strncpy(p.jpy, jpyStr.c_str(), SIZE);
        p.jpy[SIZE - 1] = '\0';
        
        strncpy(p.rub, rubStr.c_str(), SIZE);
        p.rub[SIZE - 1] = '\0';
        
        strncpy(p.date_acquired, dateStr.c_str(), DATE_SIZE);
        p.date_acquired[DATE_SIZE - 1] = '\0';

        // Escreve no arquivo binário
        binWriter.write(reinterpret_cast<char*>(&p), sizeof(Price));
    }

    csvFile.close();
    binWriter.close();
    cout << "Conversão de preços concluída com sucesso para " << binPath << endl;
} 