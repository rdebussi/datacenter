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
        // Encontra a posição correta para inserir mantendo a ordem
        BTreeIndexEntry newEntry{playerId, offset};
        auto it = std::lower_bound(entries.begin(), entries.end(), newEntry);
        
        // Se o ID já existe, atualiza o offset
        if (it != entries.end() && it->playerId == playerId) {
            std::cout << "AVISO: Atualizando offset para PlayerID existente: " << playerId << std::endl;
            it->offset = offset;
            return;
        }
        
        // Insere na posição correta
        entries.insert(it, newEntry);
    }

    void build() {
        // Não precisa mais ordenar aqui, pois mantemos ordenado durante a inserção
        std::cout << "Verificando " << entries.size() << " entradas..." << std::endl;
        
        // Verifica se há duplicatas (não deve haver, pois tratamos na inserção)
        for (size_t i = 1; i < entries.size(); i++) {
            if (entries[i].playerId == entries[i-1].playerId) {
                std::cout << "AVISO: PlayerID duplicado encontrado: " << entries[i].playerId << std::endl;
            }
        }
    }

    std::streamoff search(int playerId) const {
        BTreeIndexEntry target{playerId, 0};
        auto it = std::lower_bound(entries.begin(), entries.end(), target);
        
        if (it != entries.end() && it->playerId == playerId) {
            std::cout << "Encontrado na B-Tree: PlayerID=" << it->playerId 
                     << ", Offset=" << it->offset << std::endl;
            return it->offset;
        }
        
        std::cout << "PlayerID " << playerId << " não encontrado na B-Tree" << std::endl;
        if (it != entries.end()) {
            std::cout << "Próximo ID na árvore: " << it->playerId << std::endl;
        }
        if (it != entries.begin()) {
            --it;
            std::cout << "ID anterior na árvore: " << it->playerId << std::endl;
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
        entries.reserve(10000); // Reserva espaço para evitar realocações
        
        BTreeIndexEntry entry;
        while (file.read(reinterpret_cast<char*>(&entry.playerId), sizeof(int)) &&
               file.read(reinterpret_cast<char*>(&entry.offset), sizeof(std::streamoff))) {
            insert(entry.playerId, entry.offset); // Usa o insert ordenado
        }
        file.close();
        
        std::cout << "B-Tree carregada com " << entries.size() << " entradas" << std::endl;
        if (!entries.empty()) {
            std::cout << "Primeiro PlayerID: " << entries.front().playerId << std::endl;
            std::cout << "Último PlayerID: " << entries.back().playerId << std::endl;
        }
    }

    void inspect() const {
        std::cout << "Conteúdo da B-Tree (" << entries.size() << " entradas):" << std::endl;
        for (const auto& entry : entries) {
            std::cout << "PlayerID: " << entry.playerId 
                     << " Offset: " << entry.offset << std::endl;
        }
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
