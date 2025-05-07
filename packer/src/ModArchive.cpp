#include "formats/ModArchive.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string serializeMetadataToJson(const ModMetadata& meta) {
    json j = {
        {"id", meta.id},
        {"name", meta.name},
        {"author", meta.author},
        {"version", meta.version},
        {"description", meta.description}
    };
    return j.dump(4);
}

ModMetadata parseMetadataFromJson(const std::string& str) {
    json j = json::parse(str);
    return {
        j["id"], j["name"], j["author"], j["version"], j["description"]
    };
}

void writeArchive(std::ofstream& out, std::vector<FileEntry>& files, const std::string& rootFolder) {
    // Check for required "blue" folder
    fs::path blueFolder = fs::path(rootFolder) / "blue";
    if (!fs::exists(blueFolder) || !fs::is_directory(blueFolder)) {
        throw std::runtime_error("The root folder must contain a 'blue' folder.");
        return;
    }


    const char magic[4] = {'M','O','D','P'};
    uint8_t version = 1;

    out.write(magic, sizeof(magic));
    out.write(reinterpret_cast<char*>(&version), sizeof(version));
    uint32_t count = static_cast<uint32_t>(files.size());
    out.write(reinterpret_cast<char*>(&count), sizeof(count));

    std::streampos indexStart = out.tellp();
    for (auto& f : files) {
        uint16_t pathLen = f.path.size();
        out.write(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        out.write(f.path.c_str(), pathLen);
        out.write(reinterpret_cast<char*>(&f.offset), sizeof(f.offset)); // placeholder
        out.write(reinterpret_cast<char*>(&f.size), sizeof(f.size));
        std::cout << "Reading file: " << f.path << "\n";
    }

    for (auto& f : files) {

        f.offset = static_cast<uint64_t>(out.tellp());
        if (f.content) {
            out.write(f.content->data(), f.content->size());
        } else {
            std::ifstream fin(fs::path(rootFolder) / f.path, std::ios::binary);

            if (!fin) {
                throw std::runtime_error("Failed to open file: " + (fs::path(rootFolder) / f.path).string());
                return;
            }            

            std::vector<char> buffer(f.size);
            fin.read(buffer.data(), f.size);
            out.write(buffer.data(), f.size);
        }

        std::cout << "Packed file: " << f.path << "\n";
    }

    // Backpatch offsets
    out.seekp(indexStart);
    for (const auto& f : files) {
        uint16_t pathLen = static_cast<uint16_t>(f.path.size());
        out.seekp(out.tellp()); // Ensure stream is synced

        // Skip pathLen + path
        out.seekp(out.tellp() + std::streamoff(sizeof(uint16_t) + pathLen));

        // Write actual offset
        out.write(reinterpret_cast<const char*>(&f.offset), sizeof(f.offset));

        // Skip size field (already written before)
        out.seekp(out.tellp() + std::streamoff(sizeof(uint64_t)));

        std::cout << "Finished file: " << f.path << "\n";
    }
}

std::vector<FileEntry> readFileIndex(std::ifstream& in) {
    char magic[4];
    in.read(magic, sizeof(magic));
    uint8_t version;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    uint32_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::vector<FileEntry> files;
    for (uint32_t i = 0; i < count; ++i) {
        uint16_t pathLen;
        in.read(reinterpret_cast<char*>(&pathLen), sizeof(pathLen));
        std::string path(pathLen, '\0');
        in.read(&path[0], pathLen);
    
        uint64_t offset, size;
        in.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
    
        files.push_back({path, size, offset});
    }
    
    return files;
}

std::optional<std::string> extractFileToMemory(std::ifstream& in, const FileEntry& file) {
    in.seekg(file.offset);
    std::vector<char> buffer(file.size);
    in.read(buffer.data(), file.size);
    return std::string(buffer.begin(), buffer.end());
}

void extractAllFiles(std::ifstream& in, const std::vector<FileEntry>& files, const std::string& outputFolder) {
    for (const auto& file : files) {
        in.seekg(file.offset);
        std::vector<char> buffer(file.size);
        in.read(buffer.data(), file.size);

        fs::path outPath = fs::path(outputFolder) / file.path;
        fs::create_directories(outPath.parent_path());
        std::ofstream out(outPath, std::ios::binary);
        out.write(buffer.data(), buffer.size());
    }
}
