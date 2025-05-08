#include "ModManager.hpp"
#include "packer/Packer.h"
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<Mod> getModList() {
    std::vector<Mod> modList;

    for (const auto& entry : fs::directory_iterator("./mods/")) {

        ModMetadata info = showInfo(entry.path());

        if (info.id == "") continue;
        
        Mod mod;
        mod.modData = info;
        mod.modWeight = 0;
        mod.modFilePath = entry.path();

        modList.push_back(mod);

    }

    return modList;
}