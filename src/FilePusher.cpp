#include "FilePusher.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::unordered_map<std::string, std::vector<std::string>> finalModFiles;

std::vector<std::string> getModifiedFiles(const fs::path& modDir) {
    std::vector<std::string> files;

    for (auto& entry : fs::recursive_directory_iterator(modDir)) {
        if (fs::is_regular_file(entry.path())) {
            // Save relative path (normalized)
            files.push_back(fs::relative(entry.path(), modDir).generic_string());
        }
    }

    return files;
}

std::vector<Mod> gatherNewMods() {
    fs::path modsPath = "./mods/";

    if (!fs::exists(modsPath)) {
        std::cerr << "The ./mods/ directory does not exist. Try to relaunch the program and try again!\n";
        return {};
    }

    for (const auto& mod : fs::recursive_directory_iterator(modsPath)) {
        
    }
}

uint8_t promptModWeight(const std::string& modId, json& weightJson, const std::string& configPath) {
    if (weightJson.contains(modId)) {
        return weightJson[modId];
    }

    std::cout << "Enter weight for mod \"" << modId << "\": ";
    int weightInput = -1;

    while (weightInput < 0 || weightInput > 255) {
        std::string input;
        std::getline(std::cin, input);
        try {
            weightInput = std::stoi(input);
        } catch (...) {
            weightInput = -1;
        }

        if (weightInput < 0 || weightInput > 255) {
            std::cout << "Invalid input. Please enter a number from 0 to 255: ";
        }
    }

    weightJson[modId] = weightInput;

    // Save it
    std::ofstream out(configPath);
    if (out) {
        out << weightJson.dump(4); // pretty-print
    }

    return static_cast<uint8_t>(weightInput);
}


void filterMods() {
    const std::string modDir = "./mods";
    const std::string weightCfgPath = "./configs/mods.cfg";

    std::vector<Mod> allMods = getModList(); // You already have this function
    json weightJson;

    std::ifstream in(weightCfgPath);
    if (in) {
        in >> weightJson;
    }

    // file path (relative) -> (mod ID, weight)
    std::unordered_map<std::string, std::pair<std::string, uint8_t>> fileWinners;
    std::unordered_map<std::string, std::vector<std::string>> modFileMap;

    for (Mod& mod : allMods) {
        const std::string& modId = mod.modData.id;

        uint8_t weight = promptModWeight(modId, weightJson, weightCfgPath);
        mod.modWeight = weight; // Store in the Mod object

        auto files = getModifiedFiles(mod.modFilePath);

        for (const std::string& file : files) {
            auto it = fileWinners.find(file);

            if (it == fileWinners.end() || weight > it->second.second) {
                fileWinners[file] = { modId, weight };
            }
        }
    }

    // Gather final files per mod
    for (const auto& [file, winner] : fileWinners) {
        modFileMap[winner.first].push_back(file);
    }

    // Final list of mod file paths that will be used (if needed globally)
    chosenModPaths.clear();
    for (const Mod& mod : allMods) {
        if (modFileMap.find(mod.modData.id) != modFileMap.end()) {
            chosenModPaths.push_back(mod.modFilePath);
            std::cout << "Mod \"" << mod.modData.id << "\" will apply "
                      << modFileMap[mod.modData.id].size() << " file(s).\n";
        }
    }

    finalModFiles.clear();
    for (const auto& [file, winner] : fileWinners) {
        finalModFiles[winner.first].push_back(file);
    }
}

void pushFiles(const fs::path& gamePath) {
    if (!fs::exists(gamePath)) {
        std::cerr << "Invalid game path. Aborting file push.\n";
        return;
    }

    for (const Mod& mod : getModList()) {
        const std::string& modId = mod.modData.id;
        if (finalModFiles.find(modId) == finalModFiles.end()) continue;

        fs::path modRoot = mod.modFilePath;

        for (const std::string& relativeFile : finalModFiles[modId]) {
            fs::path sourceFile = modRoot / relativeFile;
            fs::path destinationFile = gamePath / relativeFile;

            try {
                fs::create_directories(destinationFile.parent_path()); // Ensure destination dirs exist
                fs::copy_file(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
                std::cout << "Pushed: " << relativeFile << "\n";
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Failed to copy " << relativeFile << ": " << e.what() << "\n";
            }
        }
    }

    std::cout << "All selected mod files pushed successfully.\n";
}
