#include "ModManager.hpp"
#include "SetupManager.hpp"
#include "modManagerEmbedded/EmbedManager.hpp"
#include "FilePusher.hpp"
#include <iostream>

int main() {

    // Initialize config files:
    prepareFiles();

    // Get the game path from config and if nto set, prompt user
    promptGamePath();
    // Extract the Ultimate Snowdrop Modloader v1.0.5 by Patrick Mollohan 
    writeEmbeddedFile(gamePath);    

    std::cout << "Welcome to the AFoP Mod Manager!\n" <<
                 "Please enter \"help\" to see available commands.\n";
    
    std::string userInput;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput == "exit") {
            return 0;
        } else if (userInput == "help") {
            std::cout << "A:FoP Mod Manager help:\n"
                      << "scan - Scans the \"mods/\" directory for mods and prompts for mod weight if not registered yet.\n"
                      << "push - Pushes(send/extracts) the scanned mods to the blue folder in the A:FoP directory you selected at setup. (Adds the mods to the game).\n"
                      << "exit - Closes this program, clearing the cache and saving up space. Use THIS to exit for efficiency!\n\n"
                      << "list - Lists all mods in the mods folder and their data, like author, version, etc...";
            continue;
        } else if (userInput == "list") {

            for (const auto& mod : getModList()) {
                
                const ModMetadata& meta = mod.modData;

                std::cout << "___________________________________________\n"
                          << "Mod ID: " << meta.id << "\n"
                          << "Name: " << meta.name << "\n"
                          << "Author: " << meta.author << "\n"
                          << "Version: " << meta.version << "\n"
                          << "Description: " << meta.description << "\n";
            }
            
            std::cout << "___________________________________________\n";
        } else if (userInput == "push") {
            pushFiles(gamePath);
        } else if (userInput == "scan") {
            filterMods();
        }
    
    }
}