#include <iostream> //cin cout
#include <fstream> //leitura/gravação de arquivos
#include <sstream> //para dividir a linha do .csv
#include <cstring> //para usar strncpy -> copiar string para char[] com segurança
#include "player.h" // importando a struct player
using namespace std;


string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n"); //encontra o primeiro caractere não espaço
    size_t last = str.find_last_not_of(" \t\r\n");   //encontra o último caractere não espaço
    if (first == string::npos || last == string::npos) return ""; 
    return str.substr(first, (last - first + 1)); //extrai somente a part útil da string e retorna, se for branca, retorna ""
}

//útil para  inspecionar o arquivo .bin
void inspecionarBinario(const string& nomeArquivo) {
    ifstream bin(nomeArquivo, ios::binary);
    if (!bin.is_open()) {
        cerr << "Erro ao abrir " << nomeArquivo << endl;
        return;
    }

    Player p;
    int i = 0;

    while (bin.read(reinterpret_cast<char*>(&p), sizeof(Player))) {
        cout << "Registro #" << ++i << endl;
        cout << "Bytes brutos: ";
        const unsigned char* dados = reinterpret_cast<unsigned char*>(&p);
        for (size_t j = 0; j < sizeof(Player); ++j) {
            printf("%02X ", dados[j]); // imprime cada byte em hexadecimal
        }
        cout << endl;

        cout << "Interpretado:" << endl;
        cout << "ID: " << p.playerId << endl;

        // Mostra nickname com delimitador visual
        cout << "Nickname: [" << p.nickname << "]" << endl;
        cout << "Country:  [" << p.country << "]" << endl;
        cout << "----------------------------" << endl;
    }

    bin.close();
}


int main() {

    // inspecionarBinario("players.bin");
    // ---------------- gravação dos dados em binário ----------------------------- //

    ifstream csvFile("./playstation/players.csv"); //abre o arquivo para leitura
    ofstream binWriter("./bin/players.bin", ios::binary); //abre o arquivo para gravação (em binário)
    if(!csvFile.is_open()){
        cerr << "Erro ao abrir players.csv" << endl;
        return 1;
    }
    if(!binWriter.is_open()){
        cerr << "Erro ao criar players.bin" << endl;
        return 1;
    }

    string header;
    getline(csvFile, header); //usado para ignorar o cabeçalho do .csv
    string line;
    while(getline(csvFile, line)) {
        stringstream ss(line); //usa a biblioreca ss para dividir a linha usando \t (TAB) como separador
        string idStr, nicknameStr, countryStr; //processa cada linha com o separador " , "
        getline(ss, idStr, ',');
        getline(ss, nicknameStr, ',');
        getline(ss, countryStr, ',');

        //limpa e converte os dados para evitar espaços indesejados
        idStr = trim(idStr);
        nicknameStr = trim(nicknameStr);
        countryStr = trim(countryStr);

        Player p;
        p.playerId = stoi(idStr); //string para int
        strncpy(p.nickname, nicknameStr.c_str(), NICKNAME_SIZE); //copia a string para char[] garantindo que não ultrapasse o tamanho e sempre tenha um \0 no final
        p.nickname[NICKNAME_SIZE - 1] = '\0'; //Garantir término nulo, para indicar onde a string C acaba

        strncpy(p.country, countryStr.c_str(), COUNTRY_SIZE);
        p.country[COUNTRY_SIZE - 1] = '\0';

        binWriter.write(reinterpret_cast<char*>(&p), sizeof(Player)); // grava diretamente a struct Player como binário; converte o ponteiro da struct para char e grava sizeof(Player) 
        //bytes no .bin
    }
    csvFile.close();
    binWriter.close();
    cout << "Conversão concluída com sucesso para players.bin" << endl;

    // ---------------- gravação dos dados em binário ----------------------------- //

    ifstream binReader("./bin/players.bin", ios::binary);

    if(!binReader.is_open()) {
        cerr << "Erro ao abrir o arquivo players.bin" << endl;
        return 1;
    }

    Player temp;

    cout << "Lendos jogadores do arquivo binário: " << endl;

    //lê cada sctruct do binário e imprime na tela
    while(binReader.read(reinterpret_cast<char*>(&temp), sizeof(Player))) {
        cout << "ID: " << temp.playerId
             << ", Nickname: " << temp.nickname
             << ", Country: " << temp.country << endl;
    }       

    binReader.close();


    return 0;
}
