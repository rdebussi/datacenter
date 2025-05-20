#ifndef PLAYER_H
#define PLAYER_H

const int NICKNAME_SIZE = 17;
const int COUNTRY_SIZE = 27;

struct Player {
    int playerId;
    char nickname[NICKNAME_SIZE];
    char country[COUNTRY_SIZE];
};

#endif
