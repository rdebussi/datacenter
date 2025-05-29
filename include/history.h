#ifndef HISTORY_H
#define HISTORY_H

// Tamanhos máximos baseados no conteúdo do CSV
const int ACHIEVEMENT_ID_SIZE = 30; // Ex: "670209_5436079"
const int DATE_ACQUIRED_SIZE = 20;  // Ex: "2024-11-28 10:23:24" + '\0'

// Estrutura para armazenar o histórico de conquistas
struct History {
    int playerId;
    char achievementId[ACHIEVEMENT_ID_SIZE];
    char dateAcquired[DATE_ACQUIRED_SIZE];
};

#endif
