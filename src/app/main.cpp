//
// Created by mooming on 18/07/2026.
//

#include "../system/System.h"

#include <chrono>
#include <iostream>
#include <thread>


// Verify high_resolution_clock is monotonic on this platform
static_assert(
    std::chrono::high_resolution_clock::is_steady,
    "high_resolution_clock must be steady (monotonic) for accurate frame timing"
);

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
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		// Update system
		system.Update(deltaTime);

		// Simulate frame time
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
	}

	// Shutdown system
	system.Shutdown();

	return 0;
}
