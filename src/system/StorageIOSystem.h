//
// Created by mooming on 18/07/2026.
//

#pragma once

#include "StorageInfo.h"
#include "IOBenchmarkResult.h"


namespace hs::system
{

/// @brief Storage I/O system for managing storage devices and I/O operations
/// @details Provides storage information, benchmarking, and I/O optimization
class StorageIOSystem final
{
private:
	// Current storage info
	StorageInfo storageInfo;
	IOBenchmarkResult benchmarkResult;

public:
	StorageIOSystem();
	~StorageIOSystem();

	// No copy, no move
	StorageIOSystem(const StorageIOSystem&) = delete;
	StorageIOSystem& operator=(const StorageIOSystem&) = delete;
	StorageIOSystem(StorageIOSystem&&) = delete;
	StorageIOSystem& operator=(StorageIOSystem&&) = delete;

	/// @brief Initialize the storage system
	void Initialize();

	/// @brief Shutdown the storage system
	void Shutdown();

	/// @brief Get storage info for a specific path
	[[nodiscard]] const StorageInfo& GetStorageInfo() const { return storageInfo; }

	/// @brief Update the storage system (called by System::Update)
	/// @param deltaTime Time elapsed since last update (seconds)
	void Update(float deltaTime);

private:
	/// @brief Run IO benchmark and return best configuration
	/// @return test result for the given configuration values
	[[nodiscard]] IOBenchmarkResult IOBenchmark(size_t chunkSize, size_t bufferSize) const;
};

} // namespace hs::system
