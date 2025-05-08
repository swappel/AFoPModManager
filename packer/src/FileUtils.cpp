#include "fileutils/FileUtils.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::vector<FileEntry> collectFiles(const std::string& rootFolder) {
    std::vector<FileEntry> files;

    std::cout << "Collecting files...\n";
    
    try {
        for (auto& entry : fs::recursive_directory_iterator(rootFolder)) {
            if (!entry.is_regular_file()) continue;

            FileEntry file;
            file.path = fs::relative(entry.path(), rootFolder).generic_string();
            file.size = fs::file_size(entry.path());
            
            files.push_back(file);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error during directory traversal: " << e.what() << "\n";
    }

    std::cout << files.size() << " files collected!\n";
    return files;
}
