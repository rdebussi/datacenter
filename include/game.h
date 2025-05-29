#ifndef GAME_H
#define GAME_H

// Tamanhos máximos baseados em análise dos dados do CSV e margem de segurança
const int TITLE_SIZE = 60;               // Ex: "FIFA 25"
const int PLATFORM_SIZE = 10;            // Ex: "PS5", "PC", etc.
const int DEVELOPERS_SIZE = 100;         // Ex: "['Sunseeker Games']"
const int PUBLISHERS_SIZE = 100;         // Ex: "['Ratalaika Games']"
const int GENRES_SIZE = 50;              // Ex: "['Puzzle', 'RPG']"
const int SUPPORTED_LANGUAGES_SIZE = 100; // pode vir vazio ou conter várias línguas
const int RELEASE_DATE_SIZE = 11;        // "YYYY-MM-DD" + '\0'

// Estrutura para armazenar os dados de um jogo
struct Game {
    int gameId;
    char title[TITLE_SIZE];
    char platform[PLATFORM_SIZE];
    char developers[DEVELOPERS_SIZE];
    char publishers[PUBLISHERS_SIZE];
    char genres[GENRES_SIZE];
    char supported_languages[SUPPORTED_LANGUAGES_SIZE];
    char release_date[RELEASE_DATE_SIZE];
};

#endif
