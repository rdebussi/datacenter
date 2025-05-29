#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "player.h"
#include <list>
#include <vector>
#include <string>

// Estrutura para estatísticas da tabela hash
struct HashStats {
    int totalElements;
    int totalCollisions;
    size_t maxChainLength;
    float loadFactor;
};

// Classe para tabela hash com encadeamento
class HashTableChaining {
private:
    // Aumentando o tamanho da tabela para um número primo próximo a 500.000
    // (aproximadamente 1.4x o número de elementos para manter fator de carga < 0.75)
    static const int TABLE_SIZE = 524287; // Número primo
    std::vector<std::list<Player>> table;
    int numElements;
    int collisions;

    // Função hash para playerID usando multiplicação por número primo e XOR
    size_t hashFunction(int playerId) const;
    // Função hash para nickname usando FNV-1a
    size_t hashFunction(const std::string& nickname) const;

public:
    HashTableChaining();
    void insert(const Player& player);
    Player* findByPlayerId(int playerId);
    Player* findByNickname(const std::string& nickname);
    bool remove(int playerId);
    bool remove(const std::string& nickname);
    HashStats getStats() const;
};

// Classe para tabela hash com endereçamento aberto (sondagem linear)
class HashTableOpen {
private:
    // Para endereçamento aberto, precisamos de uma tabela ainda maior
    // para manter fator de carga <= 0.75
    static const int TABLE_SIZE = 786433; // Número primo maior que 1.5x o número de elementos
    struct Entry {
        Player player;
        bool isOccupied;
        bool isDeleted;
        Entry() : isOccupied(false), isDeleted(false) {}
    };

    std::vector<Entry> table;
    int numElements;
    int collisions;

    // Funções hash melhoradas
    size_t hashFunction(int playerId) const;
    size_t hashFunction(const std::string& nickname) const;
    // Sondagem quadrática em vez de linear para melhor distribuição
    size_t probe(size_t hash, int attempt) const;

public:
    HashTableOpen();
    bool insert(const Player& player);
    Player* findByPlayerId(int playerId);
    Player* findByNickname(const std::string& nickname);
    bool remove(int playerId);
    bool remove(const std::string& nickname);
    HashStats getStats() const;
};

#endif 