// btree.h
#ifndef BTREE_H
#define BTREE_H

#include <vector>
#include <algorithm>
#include <optional>
#include <fstream>
#include <iostream>
#include "../include/player.h"
#include "../include/game.h"
#include "../include/purchased_games.h"

struct BTreeIndexEntry {
    int playerId;
    std::streamoff offset;

    bool operator<(const BTreeIndexEntry& other) const {
        return playerId < other.playerId;
    }

    bool operator==(const BTreeIndexEntry& other) const {
        return playerId == other.playerId;
    }
};

class BTree {
public:
    std::vector<BTreeIndexEntry> entries;

public:
    void insert(int playerId, std::streamoff offset) {
        BTreeIndexEntry newEntry{playerId, offset};
        auto it = std::lower_bound(entries.begin(), entries.end(), newEntry);
        
        if (it != entries.end() && it->playerId == playerId) {
            it->offset = offset;
            return;
        }
        
        entries.insert(it, newEntry);
    }

    void build() {
        // Verifica duplicatas apenas em modo debug
        #ifdef DEBUG
        for (size_t i = 1; i < entries.size(); i++) {
            if (entries[i].playerId == entries[i-1].playerId) {
                std::cout << "AVISO: PlayerID duplicado encontrado: " << entries[i].playerId << std::endl;
            }
        }
        #endif
    }

    std::streamoff search(int playerId) const {
        BTreeIndexEntry target{playerId, 0};
        auto it = std::lower_bound(entries.begin(), entries.end(), target);
        
        if (it != entries.end() && it->playerId == playerId) {
            return it->offset;
        }
        
        return -1;
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir arquivo de índice: " << filename << std::endl;
            return;
        }

        entries.clear();
        entries.reserve(10000);
        
        BTreeIndexEntry entry;
        while (file.read(reinterpret_cast<char*>(&entry.playerId), sizeof(int)) &&
               file.read(reinterpret_cast<char*>(&entry.offset), sizeof(std::streamoff))) {
            insert(entry.playerId, entry.offset);
        }
        file.close();
    }

    void inspect() const {
        std::cout << "Conteúdo da B-Tree (" << entries.size() << " entradas):" << std::endl;
        for (const auto& entry : entries) {
            std::cout << "PlayerID: " << entry.playerId 
                     << " Offset: " << entry.offset << std::endl;
        }
    }

    size_t size() const {
        return entries.size();
    }
};

// Função utilitária para construir a árvore a partir do arquivo de PurchasedGames
inline BTree buildBTreeFromPurchasedGames(const std::string& filename) {
    BTree btree;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de PurchasedGames: " << filename << std::endl;
        return btree;
    }

    while (file) {
        long pos = file.tellg();
        int playerId;
        int size;

        if (!file.read(reinterpret_cast<char*>(&playerId), sizeof(int))) break;
        if (!file.read(reinterpret_cast<char*>(&size), sizeof(int))) break;

        file.seekg(size * sizeof(int), std::ios::cur); // Pula os gameIds

        btree.insert(playerId, pos);
    }

    btree.build();
    file.close();
    return btree;
}

#endif
