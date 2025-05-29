#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "converter.h"
#include "../utils.h" // para trim() e lerCampoCSV()
#include "../../include/history.h"

using namespace std;

void converterCSVparaBinario_History(const string& csvPath, const string& binPath) {
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
    getline(csvFile, header); // Pula o cabeçalho

    int lineNum = 1;
    bool erro = false;

    while (csvFile.good() && !csvFile.eof()) {
        lineNum++;

        char nextChar = csvFile.peek();
        if (nextChar == '\n' || nextChar == '\r') {
            csvFile.get();
            if (nextChar == '\r' && csvFile.peek() == '\n') {
                csvFile.get();
            }
            continue;
        }

        string playerIdStr = lerCampoCSV(csvFile, erro);
        if (erro || csvFile.eof()) break;

        string achievementIdStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler achievementId na linha " << lineNum << endl;
            continue;
        }

        string dateStr = lerCampoCSV(csvFile, erro);
        if (erro) {
            cerr << "Erro ao ler dateAcquired na linha " << lineNum << endl;
            continue;
        }

        // Limpeza dos campos
        playerIdStr = trim(playerIdStr);
        achievementIdStr = trim(achievementIdStr);
        dateStr = trim(dateStr);

        if (playerIdStr.empty() || achievementIdStr.empty() || dateStr.empty()) {
            cerr << "Linha " << lineNum << " inválida: campo vazio." << endl;
            continue;
        }

        History h;
        try {
            h.playerId = stoi(playerIdStr);
        } catch (...) {
            cerr << "Erro ao converter playerId na linha " << lineNum << ": " << playerIdStr << endl;
            continue;
        }

        strncpy(h.achievementId, achievementIdStr.c_str(), ACHIEVEMENT_ID_SIZE);
        h.achievementId[ACHIEVEMENT_ID_SIZE - 1] = '\0';

        strncpy(h.dateAcquired, dateStr.c_str(), DATE_ACQUIRED_SIZE);
        h.dateAcquired[DATE_ACQUIRED_SIZE - 1] = '\0';

        binWriter.write(reinterpret_cast<char*>(&h), sizeof(History));
    }

    csvFile.close();
    binWriter.close();
    cout << "Conversão de histórico concluída com sucesso para " << binPath << endl;
}
