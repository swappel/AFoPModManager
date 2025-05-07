#include "fileutils/FileUtils.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::vector<FileEntry> collectFiles(const std::string& rootFolder) {
    std::vector<FileEntry> files;
    std::cout << "Collecting files...\n";

    for (auto& entry : fs::recursive_directory_iterator(rootFolder)) {
        if (!entry.is_regular_file()) continue;

        FileEntry file;
        file.path = fs::relative(entry.path(), rootFolder).generic_string();
        file.size = fs::file_size(entry.path());
        files.push_back(file);
    }

    std::cout << files.size() << " files collected!\n";

    return files;
}
