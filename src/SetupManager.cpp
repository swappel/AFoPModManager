#include "SetupManager.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void prepareFiles() {
    fs::path configPath = "./configs";
    fs::path modStorage = "./configs/mods.cfg";

    if (!fs::exists(configPath)) {
        fs::create_directory(configPath);
    }
    
    std::ifstream in("modStorage", std::ios::binary);

}