#include <iostream>
#include <chrono>
#include <thread>

#include "System.h"

int main(int argc, char* argv[])
{
    std::cout << "HS Edit - A modular TUI text editor" << std::endl;
    std::cout << "Version 0.1.0" << std::endl;
    std::cout << std::endl;
    
    // Initialize system
    auto& system = hs::system::System::GetInstance();
    system.Initialize();
    
    // Main event loop
    using Clock = std::chrono::high_resolution_clock;
    auto lastTime = Clock::now();
    
    std::cout << "System initialized. Press Ctrl+C to exit." << std::endl;
    
    while (true)
    {
        auto currentTime = Clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update system
        system.Update(deltaTime);
        
        // Simulate frame time
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }
    
    // Shutdown system
    system.Shutdown();
    
    return 0;
}
