#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <optional>

struct ModMetadata {
    std::string id;
    std::string name;
    std::string author;
    std::string version;
    std::string description;
};

struct FileEntry {
    std::string path;
    uint64_t size;
    uint64_t offset;
    std::optional<std::vector<char>> content; // if in-memory, like mod.json
};

std::string serializeMetadataToJson(const ModMetadata& meta);
ModMetadata parseMetadataFromJson(const std::string& json);

void writeArchive(std::ofstream& out, std::vector<FileEntry>& files, const std::string& rootFolder);
std::vector<FileEntry> readFileIndex(std::ifstream& in);
std::optional<std::string> extractFileToMemory(std::ifstream& in, const FileEntry& file);
void extractAllFiles(std::ifstream& in, const std::vector<FileEntry>& files, const std::string& outputFolder);