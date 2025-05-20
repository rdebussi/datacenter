#include <iostream>
#include "converter/converter.h"
#include "inspect/inspect.h" // agora separado
using namespace std;

int main() {
    //converterCSVparaBinario_Player("../playstation/players.csv", "../bin/players.bin");
    cout << "_______________________________________=*=_______________________________________" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                             Datacenter Playstation                            |" << endl;
    cout << "|-------------------------------------------------------------------------------|" << endl;
    cout << "|                              1 - Show all Players                             |" << endl;
    cout << "|                              2 - Show all Games                               |" << endl;
    cout << "|                                                                               |" << endl;
    cout << "|                                                                               |" << endl;
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
        default:
            cout << "Entrada inválida!" << endl;
    }

    //converterCSVparaBinario_Game("playstation/games.csv", "bin/games.bin");
    //cout << "\nConteúdo do arquivo players.bin:" << endl;

    return 0;
}
