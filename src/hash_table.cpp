#include "../include/hash_table.h"
#include <cstring>
#include <algorithm>
#include <cctype>

// Função auxiliar para normalizar string (remover case e acentos)
void normalizeString(const char* input, char* output) {
    unsigned char c;
    while ((c = *input++)) {
        c = tolower(c);
        if (isalnum(c)) {
            *output++ = c;
        }
    }
    *output = '\0';
}

// Função auxiliar para comparar strings normalizadas
bool compareStrings(const char* str1, const char* str2) {
    char normalized1[NICKNAME_SIZE];
    char normalized2[NICKNAME_SIZE];
    
    normalizeString(str1, normalized1);
    normalizeString(str2, normalized2);
    
    return strcmp(normalized1, normalized2) == 0;
}

// Implementação HashTableChaining

HashTableChaining::HashTableChaining() : table(TABLE_SIZE), numElements(0), collisions(0) {}

size_t HashTableChaining::hashFunction(int playerId) const {
    // Multiplicação por números primos grandes e XOR para melhor distribuição
    const size_t prime1 = 73856093;
    const size_t prime2 = 19349669;
    return ((playerId * prime1) ^ (playerId * prime2)) % TABLE_SIZE;
}

size_t HashTableChaining::hashFunction(const std::string& nickname) const {
    char normalized[NICKNAME_SIZE];
    normalizeString(nickname.c_str(), normalized);
    
    // Implementação do algoritmo FNV-1a com a string normalizada
    size_t hash = 14695981039346656037ULL; // FNV offset basis
    for (char c : std::string(normalized)) {
        hash ^= static_cast<size_t>(c);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash % TABLE_SIZE;
}

void HashTableChaining::insert(const Player& player) {
    size_t index = hashFunction(player.playerId);  // Usando ID em vez do nickname
    
    if (!table[index].empty()) {
        collisions++;
    }
    
    table[index].push_back(player);
    numElements++;
}

Player* HashTableChaining::findByPlayerId(int playerId) {
    size_t index = hashFunction(playerId);
    for (auto& player : table[index]) {
        if (player.playerId == playerId) {
            return &player;
        }
    }
    return nullptr;
}

Player* HashTableChaining::findByNickname(const std::string& nickname) {
    for (auto& bucket : table) {
        for (auto& player : bucket) {
            if (compareStrings(player.nickname, nickname.c_str())) {
                return &player;
            }
        }
    }
    return nullptr;
}

bool HashTableChaining::remove(int playerId) {
    size_t index = hashFunction(playerId);
    auto& list = table[index];
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->playerId == playerId) {
            list.erase(it);
            numElements--;
            return true;
        }
    }
    return false;
}

bool HashTableChaining::remove(const std::string& nickname) {
    size_t index = hashFunction(nickname);
    auto& list = table[index];
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (compareStrings(it->nickname, nickname.c_str())) {
            list.erase(it);
            numElements--;
            return true;
        }
    }
    return false;
}

HashStats HashTableChaining::getStats() const {
    HashStats stats;
    stats.totalElements = numElements;
    stats.totalCollisions = collisions;
    stats.loadFactor = static_cast<float>(numElements) / TABLE_SIZE;
    
    stats.maxChainLength = 0;
    for (const auto& list : table) {
        if (list.size() > stats.maxChainLength) {
            stats.maxChainLength = list.size();
        }
    }
    
    return stats;
}

// Implementação HashTableOpen

HashTableOpen::HashTableOpen() : table(TABLE_SIZE), numElements(0), collisions(0) {}

size_t HashTableOpen::hashFunction(int playerId) const {
    // Multiplicação por números primos diferentes dos usados no encadeamento
    const size_t prime1 = 73856093;
    const size_t prime2 = 19349669;
    const size_t prime3 = 83492791;
    return ((playerId * prime1) ^ (playerId * prime2) ^ (playerId * prime3)) % TABLE_SIZE;
}

size_t HashTableOpen::hashFunction(const std::string& nickname) const {
    char normalized[NICKNAME_SIZE];
    normalizeString(nickname.c_str(), normalized);
    
    // Implementação do algoritmo FNV-1a com a string normalizada
    size_t hash = 14695981039346656037ULL; // FNV offset basis
    for (char c : std::string(normalized)) {
        hash ^= static_cast<size_t>(c);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash % TABLE_SIZE;
}

size_t HashTableOpen::probe(size_t hash, int attempt) const {
    // Sondagem quadrática: h(k,i) = (h(k) + c1*i + c2*i^2) % m
    // Usando c1 = c2 = 0.5 para garantir que todas as posições sejam visitadas
    const size_t c1 = 1;
    const size_t c2 = 1;
    return (hash + c1 * attempt + c2 * attempt * attempt) % TABLE_SIZE;
}

bool HashTableOpen::insert(const Player& player) {
    if (numElements >= TABLE_SIZE) return false;
    
    size_t index = hashFunction(player.playerId);
    int attempt = 0;
    
    while (attempt < TABLE_SIZE) {
        size_t currentIndex = probe(index, attempt);
        
        if (!table[currentIndex].isOccupied || table[currentIndex].isDeleted) {
            if (attempt > 0) collisions++;
            table[currentIndex].player = player;
            table[currentIndex].isOccupied = true;
            table[currentIndex].isDeleted = false;
            numElements++;
            return true;
        }
        
        attempt++;
    }
    
    return false;
}

Player* HashTableOpen::findByPlayerId(int playerId) {
    size_t index = hashFunction(playerId);
    int attempt = 0;
    
    while (attempt < TABLE_SIZE) {
        size_t currentIndex = probe(index, attempt);
        
        if (!table[currentIndex].isOccupied) {
            if (!table[currentIndex].isDeleted) return nullptr;
        } else if (table[currentIndex].player.playerId == playerId) {
            return &table[currentIndex].player;
        }
        
        attempt++;
    }
    
    return nullptr;
}

Player* HashTableOpen::findByNickname(const std::string& nickname) {
    size_t index = hashFunction(nickname);
    int attempt = 0;
    
    while (attempt < TABLE_SIZE) {
        size_t currentIndex = probe(index, attempt);
        
        if (!table[currentIndex].isOccupied) {
            if (!table[currentIndex].isDeleted) return nullptr;
        } else if (compareStrings(table[currentIndex].player.nickname, nickname.c_str())) {
            return &table[currentIndex].player;
        }
        
        attempt++;
    }
    
    return nullptr;
}

bool HashTableOpen::remove(int playerId) {
    size_t index = hashFunction(playerId);
    int attempt = 0;
    
    while (attempt < TABLE_SIZE) {
        size_t currentIndex = probe(index, attempt);
        
        if (!table[currentIndex].isOccupied) {
            if (!table[currentIndex].isDeleted) return false;
        } else if (table[currentIndex].player.playerId == playerId) {
            table[currentIndex].isDeleted = true;
            numElements--;
            return true;
        }
        
        attempt++;
    }
    
    return false;
}

bool HashTableOpen::remove(const std::string& nickname) {
    size_t index = hashFunction(nickname);
    int attempt = 0;
    
    while (attempt < TABLE_SIZE) {
        size_t currentIndex = probe(index, attempt);
        
        if (!table[currentIndex].isOccupied) {
            if (!table[currentIndex].isDeleted) return false;
        } else if (compareStrings(table[currentIndex].player.nickname, nickname.c_str())) {
            table[currentIndex].isDeleted = true;
            numElements--;
            return true;
        }
        
        attempt++;
    }
    
    return false;
}

HashStats HashTableOpen::getStats() const {
    HashStats stats;
    stats.totalElements = numElements;
    stats.totalCollisions = collisions;
    stats.loadFactor = static_cast<float>(numElements) / TABLE_SIZE;
    stats.maxChainLength = 1; // Não aplicável para endereçamento aberto
    return stats;
} 