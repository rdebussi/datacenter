#include <fstream>
#include <iostream>
#include <cstring>
#include "../include/achievement_index.h"

using namespace std;

AchievementIndex::AchievementIndex(const string& historyFile, const string& achievementFile) 
    : historyPath(historyFile), achievementPath(achievementFile) {}

void AchievementIndex::buildIndex() {
    cout << "Construindo índice de conquistas..." << endl;
    buildAchievementIndex();
    
    cout << "Construindo índice de histórico..." << endl;
    buildHistoryIndex();
}

void AchievementIndex::buildAchievementIndex() {
    ifstream achievementFile(achievementPath, ios::binary);
    if (!achievementFile) {
        cerr << "Erro ao abrir arquivo de conquistas" << endl;
        return;
    }

    // Limpa o índice atual
    achievementOffsets.clear();

    Achievement achievement;
    long offset = 0;
    
    while (achievementFile.read(reinterpret_cast<char*>(&achievement), sizeof(Achievement))) {
        // Armazena o offset para esta conquista
        achievementOffsets[achievement.achievementId] = offset;
        offset = achievementFile.tellg();
    }

    achievementFile.close();
    cout << "Índice de conquistas construído com " << achievementOffsets.size() << " conquistas" << endl;
}

void AchievementIndex::buildHistoryIndex() {
    ifstream historyFile(historyPath, ios::binary);
    if (!historyFile) {
        cerr << "Erro ao abrir arquivo de histórico" << endl;
        return;
    }

    // Limpa o índice atual
    playerAchievementOffsets.clear();

    History history;
    long offset = 0;
    
    while (historyFile.read(reinterpret_cast<char*>(&history), sizeof(History))) {
        playerAchievementOffsets[history.playerId].push_back(offset);
        offset = historyFile.tellg();
    }

    historyFile.close();
    cout << "Índice de histórico construído com " << playerAchievementOffsets.size() << " jogadores" << endl;
}

void AchievementIndex::saveIndex(const string& indexFile) {
    ofstream outFile(indexFile, ios::binary);
    if (!outFile) {
        cerr << "Erro ao criar arquivo de índice" << endl;
        return;
    }

    // Salva o índice de jogadores
    size_t numPlayers = playerAchievementOffsets.size();
    outFile.write(reinterpret_cast<char*>(&numPlayers), sizeof(numPlayers));

    for (const auto& pair : playerAchievementOffsets) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
        
        size_t numOffsets = pair.second.size();
        outFile.write(reinterpret_cast<char*>(&numOffsets), sizeof(numOffsets));
        outFile.write(reinterpret_cast<const char*>(pair.second.data()), 
                     numOffsets * sizeof(long));
    }

    // Salva o índice de conquistas
    size_t numAchievements = achievementOffsets.size();
    outFile.write(reinterpret_cast<char*>(&numAchievements), sizeof(numAchievements));

    for (const auto& pair : achievementOffsets) {
        size_t idLen = pair.first.length();
        outFile.write(reinterpret_cast<const char*>(&idLen), sizeof(idLen));
        outFile.write(pair.first.c_str(), idLen);
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(long));
    }

    outFile.close();
}

void AchievementIndex::loadIndex(const string& indexFile) {
    ifstream inFile(indexFile, ios::binary);
    if (!inFile) {
        cerr << "Erro ao abrir arquivo de índice" << endl;
        return;
    }

    // Limpa os índices atuais
    playerAchievementOffsets.clear();
    achievementOffsets.clear();

    // Carrega o índice de jogadores
    size_t numPlayers;
    inFile.read(reinterpret_cast<char*>(&numPlayers), sizeof(numPlayers));

    for (size_t i = 0; i < numPlayers; i++) {
        int playerId;
        inFile.read(reinterpret_cast<char*>(&playerId), sizeof(playerId));
        
        size_t numOffsets;
        inFile.read(reinterpret_cast<char*>(&numOffsets), sizeof(numOffsets));
        
        vector<long> offsets(numOffsets);
        inFile.read(reinterpret_cast<char*>(offsets.data()), 
                   numOffsets * sizeof(long));
        
        playerAchievementOffsets[playerId] = move(offsets);
    }

    // Carrega o índice de conquistas
    size_t numAchievements;
    inFile.read(reinterpret_cast<char*>(&numAchievements), sizeof(numAchievements));

    for (size_t i = 0; i < numAchievements; i++) {
        size_t idLen;
        inFile.read(reinterpret_cast<char*>(&idLen), sizeof(idLen));
        
        string achievementId(idLen, '\0');
        inFile.read(&achievementId[0], idLen);
        
        long offset;
        inFile.read(reinterpret_cast<char*>(&offset), sizeof(long));
        
        achievementOffsets[achievementId] = offset;
    }

    inFile.close();
    cout << "Índices carregados com " << playerAchievementOffsets.size() 
         << " jogadores e " << achievementOffsets.size() << " conquistas" << endl;
}

vector<AchievementDetail> AchievementIndex::getPlayerAchievements(int playerId) {
    vector<AchievementDetail> achievements;
    
    auto it = playerAchievementOffsets.find(playerId);
    if (it == playerAchievementOffsets.end()) {
        return achievements;
    }

    ifstream historyFile(historyPath, ios::binary);
    ifstream achievementFile(achievementPath, ios::binary);
    
    if (!historyFile || !achievementFile) {
        cerr << "Erro ao abrir arquivos binários" << endl;
        return achievements;
    }

    for (long offset : it->second) {
        History history = readHistoryFromFile(historyFile, offset);
        
        // Busca o offset da conquista no índice
        auto achievementIt = achievementOffsets.find(history.achievementId);
        if (achievementIt != achievementOffsets.end()) {
            Achievement achievement = readAchievementFromFile(achievementFile, achievementIt->second);
            
            AchievementDetail detail;
            strncpy(detail.achievementId, achievement.achievementId, ACHIEVEMENT_SIZE);
            strncpy(detail.title, achievement.title, TITLE_SIZE);
            strncpy(detail.description, achievement.description, DESCRIPTION_SIZE);
            strncpy(detail.rarity, achievement.rarity, RARITY_SIZE);
            strncpy(detail.dateAcquired, history.dateAcquired, DATE_ACQUIRED_SIZE);
            
            achievements.push_back(detail);
        }
    }

    historyFile.close();
    achievementFile.close();
    return achievements;
}

History AchievementIndex::readHistoryFromFile(ifstream& file, long offset) {
    History history;
    file.seekg(offset);
    file.read(reinterpret_cast<char*>(&history), sizeof(History));
    return history;
}

Achievement AchievementIndex::readAchievementFromFile(ifstream& file, long offset) {
    Achievement achievement;
    file.seekg(offset);
    file.read(reinterpret_cast<char*>(&achievement), sizeof(Achievement));
    return achievement;
}