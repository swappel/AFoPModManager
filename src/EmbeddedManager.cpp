#include "modManagerEmbedded/EmbedManager.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

#include "versiondll.h"

namespace fs = std::filesystem;

void writeEmbeddedFile(const std::string& outputPath) {
    fs::path outPath = outputPath;

    // Extract version.dll if it doesn't exist
    if (!fs::exists(outPath / "version.dll")) {
        std::ofstream out(outPath / "version.dll", std::ios::binary);
        if (!out) {
            std::cerr << "Failed to extract Modloader(version.dll)!!! Cannot run mods this way! Maybe download manually.\n";
            return;
        }
        out.write(reinterpret_cast<const char*>(version_dll), version_dll_len);
        out.close();
        std::cout << "Extracted Ultimate Snowdrop Modmanager v1.0.5 (version.dll)\n";
    }

    // Extract version.ini if it doesn't exist
    if (!fs::exists(outPath / "version.ini")) {
        std::ofstream out(outPath / "version.ini", std::ios::binary);
        if (!out) {
            std::cerr << "Failed to extract Modloader(version.ini)!!! Cannot run mods this way! Maybe download manually.\n";
            return;
        }
        out.write(reinterpret_cast<const char*>(version_ini), version_ini_len);
        out.close();
        std::cout << "Extracted Ultimate Snowdrop Modmanager v1.0.5 (version.ini)\n";
    }
}