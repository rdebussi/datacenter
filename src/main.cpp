#include <iostream>
#include "converter/converter.h"
#include "inspect/inspect.h" // agora separado
using namespace std;

int main() {
    //converterCSVparaBinario_Game("playstation/games.csv", "bin/games.bin");
    //converterCSVparaBinario_Player("../playstation/players.csv", "../bin/players.bin");
    //converterCSVparaBinario_Price("playstation/prices.csv", "bin/prices.bin");
    //converterCSVparaBinario_Achievement("playstation/achievements.csv", "bin/achievements.bin");
    cout << "_______________________________________=*=_______________________________________" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                             Datacenter Playstation                            |" << endl;
    cout << "|-------------------------------------------------------------------------------|" << endl;
    cout << "|                              1 - Show all Players                             |" << endl;
    cout << "|                              2 - Show all Games                               |" << endl;
    cout << "|                              3 - Show all Prices                              |" << endl;
    cout << "|                              4 - Show all Achievements                        |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|_______________________________________________________________________________|" << endl;
    
    int option;
    cin >> option;

    switch (option) {
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
        default:
            cout << "Entrada inválida!" << endl;
    }

    //cout << "\nConteúdo do arquivo players.bin:" << endl;

    return 0;
}
