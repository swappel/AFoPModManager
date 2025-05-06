#include <string>
#include <vector>
#include <filesystem>
#include "formats/ModArchive.h"

struct Mod {
    int modWeight;
    std::vector<std::filesystem::path> modFilePaths;
    ModMetadata modData;
};

std::vector<Mod> getModList();

void pushMods();
void enableMod(std::string& modId);
void disableMod(std::string& modId);
void reloadMods();
