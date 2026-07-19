//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <memory>
#include <mutex>

#include "StorageIOSystem.h"
#include "VirtualTextBufferSystem.h"

namespace hs::system
{

class StorageIOSystem;
class VirtualTextBufferSystem;


/// @brief Base system singleton for hsedit
/// @details Manages subsystems and provides initialization/shutdown lifecycle
class System final
{
private:
	// Subsystems
	StorageIOSystem storageIOSystem;
	VirtualTextBufferSystem virtualTextBufferSystem;

	// State
	bool initialized_;

public:
	/// @brief Get the singleton instance
	[[nodiscard]] static System& GetInstance();

	System(const System&) = delete;
	System& operator=(const System&) = delete;

	/// @brief Initialize the system and all subsystems
	void Initialize();

	/// @brief Shutdown the system and all subsystems
	void Shutdown();

	/// @brief Update the system and all subsystems
	/// @param deltaTime Time elapsed since last update (seconds)
	void Update(float deltaTime);

	/// @brief Get the storage I/O system
	[[nodiscard]] StorageIOSystem& GetStorageIOSystem();

	/// @brief Get the virtual text buffer system
	[[nodiscard]] VirtualTextBufferSystem& GetVirtualTextBufferSystem();

	/// @brief Check if system is initialized
	[[nodiscard]] bool IsInitialized() const;

private:
	System();
	~System();

	// Allow move
	System(System&&);
	System& operator=(System&&);
};

} // namespace hs::system
