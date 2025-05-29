#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <cstdlib> // para system()
#include "src/converter/converter.h"
#include "src/inspect/inspect.h"
#include "include/purchased_games.h"

using namespace std;

int main() {
    // Configura a codificação para UTF-8
    setlocale(LC_ALL, "pt_BR.UTF-8");
    cout.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    int option;

    do {
        cout << "_______________________________________=*=_______________________________________" << endl;
        cout << "|                                                                               |" << endl;
        cout << "|                             Datacenter Playstation                            |" << endl;
        cout << "|-------------------------------------------------------------------------------|" << endl;
        cout << "|                              1 - Show all Players                             |" << endl;
        cout << "|                              2 - Show all Games                               |" << endl;
        cout << "|                              3 - Show all Prices                              |" << endl;
        cout << "|                              4 - Show all Achievements                        |" << endl;
        cout << "|                              5 - Show all Purchased Games                     |" << endl;
        cout << "|-------------------------------------------------------------------------------|" << endl;
        cout << "|                              6 - Construir Árvore B                           |" << endl;
        cout << "|                              7 - Consultar biblioteca de jogador              |" << endl;
        cout << "|                                                                               |" << endl;
        cout << "|                              0 - Exit                                         |" << endl;
        cout << "|_______________________________________________________________________________|" << endl;
        cout << "\nEscolha uma opção: ";
        cin >> option;

        switch (option) {
            case 0:
                cout << "Saindo..." << endl;
                break;
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
            case 6:
                cout << "\nConstruindo árvore B...";
                system("./build_btree");
                break;
            case 7:
                cout << "\nConsultando biblioteca de jogador...\n";
                system("./query_library");
                break;
            default:
                cout << "Entrada inválida!" << endl;
        }

        cout << "\nPressione ENTER para continuar...";
        cin.ignore();
        cin.get();

    } while (option != 0);

    return 0;
}
