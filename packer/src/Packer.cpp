#include "packer/Packer.h"
#include "fileutils/FileUtils.h"
#include "formats/ModArchive.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

void pack(const std::string& inputFolder, const std::string& outputFile) {
    fs::path root = fs::path(inputFolder);

    // Ensure "blue" folder exists
    fs::path blueDir = root / "blue";
    if (!fs::exists(blueDir) || !fs::is_directory(blueDir)) {
        throw std::runtime_error("Input folder must contain a 'blue' subfolder.");
    }

    // Read and parse mod.json
    fs::path modJsonPath = root / "mod.json";
    if (!fs::exists(modJsonPath)) {
        throw std::runtime_error("Missing required 'mod.json' file in root folder.");
    }

    std::ifstream metaFile(modJsonPath);
    if (!metaFile) throw std::runtime_error("Cannot open mod.json.");

    std::vector<char> jsonBuffer((std::istreambuf_iterator<char>(metaFile)), {});
    ModMetadata meta = parseMetadataFromJson(std::string(jsonBuffer.begin(), jsonBuffer.end()));

    // Collect files
    std::vector<FileEntry> files = collectFiles(inputFolder);

    // Add mod.json as virtual file (in-memory)
    files.push_back({ "mod.json", static_cast<uint64_t>(jsonBuffer.size()), 0, jsonBuffer });

    // Write archive
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output file.");

    writeArchive(out, files, root);
}

void unpack(const std::string& inputFile, const std::string& outputFolder) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input file.");

    auto files = readFileIndex(in);
    extractAllFiles(in, files, outputFolder);
}

ModMetadata showInfo(const std::string& inputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input file.");

    auto files = readFileIndex(in);

    // Find mod.json entry
    auto it = std::find_if(files.begin(), files.end(), [](const FileEntry& f) {
        return f.path == "mod.json";
    });

    if (it == files.end()) {
        std::cerr << "No mod.json found in archive.\n";
        return {};
    }

    auto jsonStrOpt = extractFileToMemory(in, *it);
    if (!jsonStrOpt) {
        std::cerr << "Failed to read mod.json from archive.\n";
        return {};
    }

    return parseMetadataFromJson(*jsonStrOpt);
}
