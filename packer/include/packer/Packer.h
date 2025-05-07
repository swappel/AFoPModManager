#pragma once
#include "formats/ModArchive.h"
#include <string>

void pack(const std::string& inputFolder, const std::string& outputFile);
void unpack(const std::string& inputFile, const std::string& outputFolder);
ModMetadata showInfo(const std::string& inputFile);