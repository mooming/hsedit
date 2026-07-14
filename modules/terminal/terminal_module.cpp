#include <iostream>
#include <string>

// TODO: Include ncurses.h when ready
// #include <ncurses.h>

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--reveal") {
        std::cout << "[call] register: name=\"terminal\", version=\"1.0\"" << std::endl;
        std::cout << "[call] register_capability: capability=\"INPUT\"" << std::endl;
        std::cout << "[call] register_capability: capability=\"RENDER\"" << std::endl;
        return 0;
    }
    
    std::cout << "[Terminal Module] Initialized" << std::endl;
    
    // TODO: Initialize ncurses
    // initscr();
    // cbreak();
    // noecho();
    // keypad(stdscr, TRUE);
    
    // TODO: Main loop
    // while (true) {
    //     int key = getch();
    //     std::cout << "[call] input_received: " << key << std::endl;
    // }
    
    // TODO: Shutdown ncurses
    // endwin();
    
    return 0;
}
