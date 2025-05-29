#ifndef PURCHASED_GAMES_H
#define PURCHASED_GAMES_H

#include <vector>

struct PurchasedGames {
    int playerId;
    std::vector<int> gameIds;  // Lista de IDs dos jogos comprados
};

#endif