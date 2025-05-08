#include "SetupManager.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::string gamePath;

void prepareFiles() {
    // Creating a few required files.

    fs::path configPath = "./configs";
    fs::path modConfig = "./configs/mods.cfg";
    fs::path generalConfig = "./configs/settings.cfg";
    fs::path modStorage = "./mods";

    if (!fs::exists(configPath)) {
        fs::create_directory(configPath);
    }
    
    if (!fs::exists(modStorage)) {
        fs::create_directory(modStorage);
    }

    if (!fs::exists(modConfig)) {
        std::ofstream out(modConfig);
        out.close();
    }

    if (!fs::exists(generalConfig)) {
        std::ofstream out(generalConfig);
        out.close();
    }
}

bool isValidGamePath(const std::string& path) {

    // Check if certain fiels are present in the given directory

    fs::path gamePath = path;

    if (!fs::exists(gamePath / "afop.exe")){ 
        std::cerr << "No /afop.exe found! Invalid game path.\n";
        return false; 
    }
    if (!fs::exists(gamePath / "afop_plus.exe")) { 
        std::cerr << "No /afop_plus.exe found! Invalid game path.\n";
        return false; 
    }
    if (!fs::exists(gamePath / "xinput1_4.dll")) { 
        std::cerr << "No /xinput1_4.dll found! Invalid game path.\n";
        return false;
    }
    if (!fs::exists(gamePath / "rogue/")) { 
        std::cerr << "No /rogue/ found! Invalid game path.\n";
        return false; 
    }
    if (!fs::exists(gamePath / "dbghelp.dll")) { 
        std::cerr << "No /dbghelp.dll found! Invalid game path.\n";
        return false;
    }
    if (!fs::exists(gamePath / "version.dll")) { 
        std::cerr << "No /version.dll found! Might be invalid!\n";
    }
    if (!fs::exists(gamePath / "version.ini")) { 
        std::cerr << "No /version.ini found! Might be invalid!\n";
    }

    return true;
}

const std::string promptGamePath() {
    std::ifstream in("./configs/settings.cfg");
    std::string line;
    std::string existingPath;

    if (in) {
        while (std::getline(in, line)) {
            if (line.rfind("gamePath =", 0) == 0) {
                existingPath = line.substr(std::string("gamePath =").length());

                // Trim any whitespace
                existingPath.erase(0, existingPath.find_first_not_of(" \t"));
                existingPath.erase(existingPath.find_last_not_of(" \t") + 1);

                if (fs::exists(existingPath)) {
                    existingPath = fs::weakly_canonical(fs::path(existingPath)).string();

                    if (isValidGamePath(existingPath)) {
                        std::cout << "Game path loaded from settings: " << existingPath << "\n";
                        gamePath = existingPath;
                        return gamePath;
                    } else {
                        std::cerr << "Stored game path is invalid.\n";
                    }
                } else {
                    std::cerr << "Stored game path does not exist.\n";
                }

                break; // stop after finding first match
            }
        }
    }

    // Ask user for input if not found or invalid
    std::string userInputGamePath;
    bool isValidPath = false;

    while (!isValidPath) {
        std::cout << "Please enter your AFoP's path: ";
        std::getline(std::cin, userInputGamePath);

        if (!fs::exists(userInputGamePath)) {
            std::cerr << "This path does not exist! Retry!\n";
            continue;
        }

        userInputGamePath = fs::weakly_canonical(fs::path(userInputGamePath)).string();
        isValidPath = isValidGamePath(userInputGamePath);
    }

    std::cout << "Game path successfully set to " << userInputGamePath << "\n";

    // Save new path
    std::ofstream out("./configs/settings.cfg", std::ios::app);
    if (out) {
        out << "gamePath = " << userInputGamePath << "\n";
    } else {
        std::cerr << "Failed to open settings.cfg for writing.\n";
    }

    gamePath = userInputGamePath;
    return gamePath;
}
