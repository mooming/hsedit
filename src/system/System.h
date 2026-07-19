//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <memory>
#include <mutex>


namespace hs::system
{

class StorageIOSystem;


/// @brief Base system singleton for hsedit
/// @details Manages subsystems and provides initialization/shutdown lifecycle
class System final
{
public:
	/// @brief Get the singleton instance
	[[nodiscard]] static System& GetInstance();

	/// @brief Initialize the system and all subsystems
	void Initialize();

	/// @brief Shutdown the system and all subsystems
	void Shutdown();

	/// @brief Get the storage I/O system
	[[nodiscard]] StorageIOSystem& GetStorageIOSystem();

	/// @brief Check if system is initialized
	[[nodiscard]] bool IsInitialized() const;

private:
	System();
	~System();

	// No copy
	System(const System&) = delete;
	System& operator=(const System&) = delete;

	// Allow move
	System(System&&) noexcept;
	System& operator=(System&&) noexcept;

	// Subsystems
	std::unique_ptr<StorageIOSystem> storageIOSystem_;

	// State
	bool initialized_;
};

} // namespace hs::system
