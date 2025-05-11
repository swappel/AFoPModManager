#include "FilePusher.hpp"
#include "ModManager.hpp"
#include "nlohmann/json.hpp"
#include <packer/Packer.h>
#include <iostream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::vector<std::filesystem::path> chosenModPaths;
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

void unpackModFiles() {

    std::cout << "Starting mod caching:\n";

    fs::path cachePath = "./modcache/";

    fs::create_directory(cachePath);

    for (const auto& mod : getModList()) {
        unpack(mod.modFilePath.string(), (cachePath / mod.modData.id).string());
        std::cout << "Unpacked " << mod.modData.id << " to cache.\n";
    }

    std::cout << "All mods unpackacked and cached.\n\n";
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
    unpackModFiles();

    const std::string modDir = "./mods";
    const std::string weightCfgPath = "./configs/mods.cfg";

    std::vector<Mod> allMods = getModList(); // You already have this function
    json weightJson;

    std::ifstream in(weightCfgPath);
    if (in && in.peek() != std::ifstream::traits_type::eof()) {
        in >> weightJson;
    } else {
        weightJson = json::object(); // start with empty object
    }


    // file path (relative) -> (mod ID, weight)
    std::unordered_map<std::string, std::pair<std::string, uint8_t>> fileWinners;
    std::unordered_map<std::string, std::vector<std::string>> modFileMap;

    for (Mod& mod : allMods) {
        const std::string& modId = mod.modData.id;

        uint8_t weight = promptModWeight(modId, weightJson, weightCfgPath);
        mod.modWeight = weight; // Store in the Mod object

        fs::path cachePath = "./modcache/" + mod.modData.id;
        auto files = getModifiedFiles(cachePath);

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
            chosenModPaths.push_back("./modcache/" + mod.modData.id);
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
    std::cout << "Starting mod push to game directory:\n";
    if (!fs::exists(gamePath)) {
        std::cerr << "Invalid game path. Aborting file push.\n";
        return;
    }

    fs::path modCache = "./modcache";

    for (const Mod& mod : getModList()) {
        const std::string& modId = mod.modData.id;
        if (finalModFiles.find(modId) == finalModFiles.end()) continue;

        fs::path modRoot = modCache / modId;

        for (const std::string& relativeFile : finalModFiles[modId]) {

            fs::path sourceFile = modRoot / relativeFile;
            fs::path destinationFile = gamePath / relativeFile;
            
            if (destinationFile.filename() == "mod.json") continue;

            std::cout << "Pushed file " << destinationFile.filename() << "\n";

            try {
                fs::create_directories(destinationFile.parent_path()); // Ensure destination dirs exist
                fs::copy_file(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
                std::cout << "Pushed: " << relativeFile << "\n";
            } catch (const fs::filesystem_error& e) {
                std::cerr << "Failed to copy " << relativeFile << ": " << e.what() << "\n";
            }
        }
    }

    fs::remove_all(modCache);

    std::cout << "All selected mod files pushed successfully.\n";
}

