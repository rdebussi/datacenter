#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include "./converter.h"
#include "../../include/player.h"       
#include "../utils.h"

using namespace std;

void converterCSVparaBinario_Player(const string& csvPath, const string& binPath) {
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
        string idStr, nicknameStr, countryStr;
        getline(ss, idStr, ',');
        getline(ss, nicknameStr, ',');
        getline(ss, countryStr, ',');

        idStr = trim(idStr);
        nicknameStr = trim(nicknameStr);
        countryStr = trim(countryStr);

        Player p;
        p.playerId = stoi(idStr);
        strncpy(p.nickname, nicknameStr.c_str(), NICKNAME_SIZE);
        p.nickname[NICKNAME_SIZE - 1] = '\0';
        strncpy(p.country, countryStr.c_str(), COUNTRY_SIZE);
        p.country[COUNTRY_SIZE - 1] = '\0';

        binWriter.write(reinterpret_cast<char*>(&p), sizeof(Player));
    }

    csvFile.close();
    binWriter.close();
    cout << "Conversão concluída com sucesso para " << binPath << endl;
}
