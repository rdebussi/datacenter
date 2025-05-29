#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include "src/converter/converter.h"
#include "src/inspect/inspect.h"
#include "include/purchased_games.h"
using namespace std;

int main() {
    // Configura a codificação para UTF-8
    setlocale(LC_ALL, "pt_BR.UTF-8");
    cout.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    
    //converterCSVparaBinario_Game("playstation/games.csv", "bin/games.bin");
    //converterCSVparaBinario_Player("../playstation/players.csv", "../bin/players.bin");
    //converterCSVparaBinario_Price("playstation/prices.csv", "bin/prices.bin");
    //converterCSVparaBinario_Achievement("playstation/achievements.csv", "bin/achievements.bin");
    converterCSVparaBinario_PurchasedGames("playstation/purchased_games.csv", "bin/purchased_games.bin");
    cout << "_______________________________________=*=_______________________________________" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                             Datacenter Playstation                            |" << endl;
    cout << "|-------------------------------------------------------------------------------|" << endl;
    cout << "|                              1 - Show all Players                             |" << endl;
    cout << "|                              2 - Show all Games                               |" << endl;
    cout << "|                              3 - Show all Prices                              |" << endl;
    cout << "|                              4 - Show all Achievements                        |" << endl;
    cout << "|                              5 - Show all Purchased Games                     |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                              0 - Exit                                         |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|_______________________________________________________________________________|" << endl;
    cout << "\nEscolha uma opção: ";
    
    int option;
    cin >> option;

    switch (option) {
        case 0:
            cout << "Saindo..." << endl;
            return 0;
        case 1: 
            inspecionarBinario_Player("bin/players.bin");
            break;
        case 2: 
            InspecionarBinario_Game("bin/games.bin");
            break;
        case 3:
            InspecionarBinario_Price("bin/prices.bin");
            break;
        case 4:
            InspecionarBinario_Achievement("bin/achievements.bin");
            break;
        case 5:
            inspecionarBinario_PurchasedGames("bin/purchased_games.bin");
            break;
        default:
            cout << "Entrada inválida!" << endl;
    }

    //cout << "\nConteúdo do arquivo players.bin:" << endl;

    return 0;
}
