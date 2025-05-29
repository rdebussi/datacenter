#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include "converter.h"
#include "../../include/game.h"
#include "../utils.h"

using namespace std;

// Limpa colchetes e aspas simples
string limparCampoLista(const string& campo) {
    string limpo;
    for (char c : campo) {
        if (c != '\'' && c != '[' && c != ']') {
            limpo += c;
        }
    }
    return trim(limpo);
}

// Divide a linha CSV considerando aspas e vírgulas internas
vector<string> parseCSVLine(const string& linha) {
    vector<string> campos;
    bool dentroAspas = false;
    string campoAtual;

    for (size_t i = 0; i < linha.size(); ++i) {
        char c = linha[i];

        if (c == '"') {
            dentroAspas = !dentroAspas; // alterna estado
        } else if (c == ',' && !dentroAspas) {
            campos.push_back(trim(campoAtual));
            campoAtual.clear();
        } else {
            campoAtual += c;
        }
    }

    campos.push_back(trim(campoAtual)); // último campo
    return campos;
}

void converterCSVparaBinario_Game(const string& csvPath, const string& binPath) {
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
        vector<string> campos = parseCSVLine(line);
        if (campos.size() != 8) {
            cerr << "Linha inválida (esperado 8 campos):\n" << line << endl;
            continue;
        }

        // Limpeza
        string idStr    = trim(campos[0]);
        string titleStr = trim(campos[1]);
        string platformStr = trim(campos[2]);
        string devStr   = limparCampoLista(campos[3]);
        string pubStr   = limparCampoLista(campos[4]);
        string genresStr= limparCampoLista(campos[5]);
        string langsStr = limparCampoLista(campos[6]);
        string dateStr  = trim(campos[7]);

        // Preenchimento
        Game g;
        g.gameId = stoi(idStr);

        strncpy(g.title, titleStr.c_str(), TITLE_SIZE);
        g.title[TITLE_SIZE - 1] = '\0';

        strncpy(g.platform, platformStr.c_str(), PLATFORM_SIZE);
        g.platform[PLATFORM_SIZE - 1] = '\0';

        strncpy(g.developers, devStr.c_str(), DEVELOPERS_SIZE);
        g.developers[DEVELOPERS_SIZE - 1] = '\0';

        strncpy(g.publishers, pubStr.c_str(), PUBLISHERS_SIZE);
        g.publishers[PUBLISHERS_SIZE - 1] = '\0';

        strncpy(g.genres, genresStr.c_str(), GENRES_SIZE);
        g.genres[GENRES_SIZE - 1] = '\0';

        strncpy(g.supported_languages, langsStr.c_str(), SUPPORTED_LANGUAGES_SIZE);
        g.supported_languages[SUPPORTED_LANGUAGES_SIZE - 1] = '\0';

        strncpy(g.release_date, dateStr.c_str(), RELEASE_DATE_SIZE);
        g.release_date[RELEASE_DATE_SIZE - 1] = '\0';

        binWriter.write(reinterpret_cast<char*>(&g), sizeof(Game));
    }

    csvFile.close();
    binWriter.close();
    cout << "Conversão concluída com sucesso para " << binPath << endl;
}
