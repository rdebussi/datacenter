#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

void converterCSVparaBinario_Game(const std::string& csvFilename, const std::string& binFilename);
void converterCSVparaBinario_Player(const std::string& csvFilename, const std::string& binFilename);
void converterCSVparaBinario_Price(const std::string& csvFilename, const std::string& binFilename);
void converterCSVparaBinario_Achievement(const std::string& csvFilename, const std::string& binFilename);
void converterCSVparaBinario_PurchasedGames(const std::string& csvFilename, const std::string& binFilename);
void converterCSVparaBinario_History(const std::string& csvFilename, const std::string& binFilename);

#endif 