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
            
        }
        
    }
    
    return 0;
}