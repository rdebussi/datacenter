#ifndef PURCHASED_GAMES_H
#define PURCHASED_GAMES_H

const int PLAYERID_SIZE = 8;
const int LIBRARY_SIZE = 185218;

struct Purchased_Games {
    int playerId[PLAYERID_SIZE];
    char library[LIBRARY_SIZE];
};

#endif