#pragma once
#include <string>

extern std::string gamePath;

void prepareFiles();
const std::string promptGamePath();
bool isValidGamePath(const std::string& gamePath);