#pragma once
#include "ModManager.hpp"
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "nlohmann/json.hpp"

std::vector<Mod> gatherNewMods();
void filterMods();
uint8_t promptModWeight(const std::string& modId, nlohmann::json& weightJson, const std::string& configPath);
void pushFiles(const std::filesystem::path& gamePath);