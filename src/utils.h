// include/utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string trim(const std::string& str);
std::string lerCampoCSV(std::ifstream& file, bool& erro);

#endif
