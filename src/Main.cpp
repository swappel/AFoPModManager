#include "ModManager.hpp"
#include <iostream>

int main() {

    std::cout << "Welcome to the AFoP Mod Manager!\n" <<
                 "Please enter \"help\" to see available commands.";
    
    std::string userInput;

    while (true) {
        std::cout << "> ";
        std::cin >> userInput;

        if (userInput == "exit") {
            return 0;
        } else if (userInput == "help") {
            std::cout << "";
            continue;
        } else if (userInput == "list") {

            for (const auto& mod : getModList()) {
                
                ModMetadata meta = mod.modData;

                std::cout << "___________________________________________\n"
                          << "Mod ID: " << meta.id << "\n"
                          << "Name: " << meta.name << "\n"
                          << "Author: " << meta.author << "\n"
                          << "Version: " << meta.version << "\n"
                          << "Description: " << meta.description << "\n";
            }
            
            std::cout << "___________________________________________\n";
        }        
    }
}