#include <iostream>
#include <iomanip>
#include "../include/achievement_index.h"

using namespace std;

void displayAchievements(const vector<AchievementDetail>& achievements) {
    cout << "==================================================================================" << endl;
    cout << "|                           CONQUISTAS DO JOGADOR                                 |" << endl;
    cout << "==================================================================================" << endl;
    
    // for (const auto& achievement : achievements) {
    //     cout << "| Título: " << achievement.title << endl;
    //     cout << "| Descrição: " << achievement.description << endl;
    //     cout << "| Raridade: " << achievement.rarity << endl;
    //     cout << "| Data de conquista: " << achievement.dateAcquired << endl;
    //     cout << "----------------------------------------------------------------------------------" << endl;
    // }
    
    cout << "Total de conquistas: " << achievements.size() << endl;
}

int main() {
    // Inicializa o índice
    AchievementIndex index("bin/history.bin", "bin/achievements.bin");
    
    // Constrói o índice
    cout << "Construindo índice..." << endl;
    index.buildIndex();
    
    // Salva o índice
    cout << "Salvando índice..." << endl;
    index.saveIndex("bin/achievement_index.bin");
    
    while (true) {
        cout << "\nDigite o ID do jogador (ou 0 para sair): ";
        int playerId;
        cin >> playerId;
        
        if (playerId == 0) break;
        
        // Busca as conquistas do jogador
        vector<AchievementDetail> achievements = index.getPlayerAchievements(playerId);
        
        // Exibe as conquistas
        displayAchievements(achievements);
    }
    
    return 0;
} 