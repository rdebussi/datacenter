#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

const int ACHIEVEMENT_SIZE = 15;
const int GAME_SIZE = 7;
const int TITLE_SIZE = 60;
const int DESCRIPTION_SIZE = 150;
const int RARITY_SIZE = 10; 

struct Achievement {
    char achievementId[ACHIEVEMENT_SIZE];
    int gameId[GAME_SIZE];
    char title[TITLE_SIZE];
    char description[DESCRIPTION_SIZE];
    char rarity[RARITY_SIZE];
};

#endif