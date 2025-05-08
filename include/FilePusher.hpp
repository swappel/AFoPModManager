#pragma once
#include "ModManager.hpp"
#include <vector>
#include <filesystem>
#include <unordered_map>
#include "nlohmann/json.hpp"

extern std::vector<std::filesystem::path> chosenModPaths;

std::vector<Mod> gatherNewMods();
void filterMods(const std::unordered_map<Mod, uint8_t>& modWeightMap);
uint8_t promptModWeight(const std::string& modId, nlohmann::json& weightJson, const std::string& configPath);