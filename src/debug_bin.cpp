#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main() {
    ifstream file("bin/purchased_games.bin", ios::binary);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo" << endl;
        return 1;
    }

    // Lê os primeiros 5 registros
    for (int i = 0; i < 5; i++) {
        cout << "\nRegistro #" << i << ":" << endl;
        
        // Posição atual
        streampos pos = file.tellg();
        cout << "Offset: " << pos << endl;
        
        // Lê o playerID
        int playerId;
        file.read(reinterpret_cast<char*>(&playerId), sizeof(int));
        cout << "PlayerID (int): " << playerId << endl;
        
        // Volta 4 bytes para mostrar os bytes do playerID
        file.seekg(pos);
        unsigned char bytes[4];
        file.read(reinterpret_cast<char*>(bytes), 4);
        cout << "PlayerID bytes: ";
        for (int j = 0; j < 4; j++) {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(bytes[j]) << " ";
        }
        cout << dec << endl;
        
        // Lê o size
        int size;
        file.read(reinterpret_cast<char*>(&size), sizeof(int));
        cout << "Size: " << size << endl;
        
        // Pula os gameIds
        file.seekg(size * sizeof(int), ios::cur);
    }

    return 0;
} 