#ifndef ACHIEVEMENT_INDEX_H
#define ACHIEVEMENT_INDEX_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include "achievement.h"
#include "history.h"

// Estrutura para armazenar uma conquista com seus detalhes
struct AchievementDetail {
    char achievementId[ACHIEVEMENT_SIZE];
    char title[TITLE_SIZE];
    char description[DESCRIPTION_SIZE];
    char rarity[RARITY_SIZE];
    char dateAcquired[DATE_ACQUIRED_SIZE];
};

// Classe para gerenciar o índice de conquistas
class AchievementIndex {
private:
    std::map<int, std::vector<long>> playerAchievementOffsets; // Índice de jogadores -> offsets do history.bin
    std::unordered_map<std::string, long> achievementOffsets;  // Índice de achievementId -> offset do achievements.bin
    std::string historyPath;
    std::string achievementPath;

public:
    AchievementIndex(const std::string& historyFile, const std::string& achievementFile);
    
    // Constrói os índices iniciais
    void buildIndex();
    
    // Salva os índices em arquivo
    void saveIndex(const std::string& indexFile);
    
    // Carrega os índices do arquivo
    void loadIndex(const std::string& indexFile);
    
    // Busca todas as conquistas de um jogador
    std::vector<AchievementDetail> getPlayerAchievements(int playerId);

private:
    // Método auxiliar para ler um registro de Achievement do arquivo binário
    Achievement readAchievementFromFile(std::ifstream& file, long offset);
    
    // Método auxiliar para ler um registro de History do arquivo binário
    History readHistoryFromFile(std::ifstream& file, long offset);
    
    // Constrói o índice de conquistas
    void buildAchievementIndex();
    
    // Constrói o índice de histórico
    void buildHistoryIndex();
};

#endif