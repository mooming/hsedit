//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <mutex>


namespace hs::system
{

/// @brief Storage device information
struct StorageInfo final
{
	TFilePath devicePath;          // Device path (e.g., /dev/disk0s1)
	TFilePath mountPoint;          // Mount point (e.g., /)
	std::string deviceName;        // Device name (e.g., "Samsung SSD 970 EVO")
	std::string deviceType;        // Device type (e.g., "NVMe", "SATA SSD", "HDD")
	bool isSSD;                    // Is this an SSD?
	bool isRemovable;              // Is this a removable device?
	
	// Capacity information
	size_t totalSizeBytes;         // Total capacity in bytes
	size_t freeSizeBytes;          // Free space in bytes
	
	// Performance characteristics
	size_t optimalIOSizeBytes;     // Optimal I/O block size
	size_t maxSequentialReadMBs;   // Max sequential read speed (MB/s)
	size_t maxSequentialWriteMBs;  // Max sequential write speed (MB/s)
	size_t randomReadIOPS;         // Random read IOPS
	size_t randomWriteIOPS;        // Random write IOPS
	
	// Wear information (for SSDs)
	double percentageUsed;         // Percentage of life used (0.0 - 1.0)
	
	// File system information
	std::string fileSystemType;    // File system type (e.g., "APFS", "NTFS", "ext4")
	size_t blockSizeBytes;         // File system block size
};


/// @brief Benchmark result for a specific configuration
struct IOBenchmarkResult final
{
	std::string configName;
	size_t pageSizeInBytes;
	size_t cacheSizeInBytes;
	
	// Performance metrics
	double openTimeMs;             // Time to open a file (ms)
	double readThroughputMBs;      // Sequential read throughput (MB/s)
	double writeThroughputMBs;     // Sequential write throughput (MB/s)
	double randomReadLatencyUs;    // Random read latency (us)
	double randomWriteLatencyUs;   // Random write latency (us)
	size_t memoryUsageBytes;       // Estimated memory usage
	
	// Composite score (lower is better)
	double score;
	
	// Recommendation
	std::string recommendation;
};


/// @brief Benchmark preset configurations
enum class IOBenchmarkPreset
{
	Mobile,         // Low-memory devices (mobile, embedded)
	Desktop,        // Standard desktop editor
	LargeFile,      // Specialized for large files (>1GB)
	SSDOptimized,   // Optimized for SSD storage
	HDDOptimized,   // Optimized for HDD storage
	AutoDetect      // Auto-detect best configuration
};


/// @brief Storage I/O system for managing storage devices and I/O operations
/// @details Provides storage information, benchmarking, and I/O optimization
class StorageIOSystem final
{
public:
	StorageIOSystem();
	~StorageIOSystem();

	/// @brief Initialize the storage system
	void Initialize();

	/// @brief Shutdown the storage system
	void Shutdown();

	/// @brief Get all storage devices
	[[nodiscard]] std::vector<StorageInfo> GetStorageDevices() const;

	/// @brief Get storage info for a specific path
	[[nodiscard]] StorageInfo GetStorageInfoForPath(const TFilePath& path) const;

	/// @brief Run IO benchmark and return best configuration
	/// @param preset Preset to use for benchmarking
	/// @param testFilePath Path to test file (0 bytes = create temp file)
	/// @param testFileSizeBytes Size of test file in bytes (0 = use existing)
	/// @param numRuns Number of benchmark runs
	/// @return Best configuration result
	[[nodiscard]] IOBenchmarkResult IOBenchmark(
		IOBenchmarkPreset preset = IOBenchmarkPreset::AutoDetect,
		const TFilePath& testFilePath = "",
		size_t testFileSizeBytes = 0,
		int numRuns = 3
	) const;

	/// @brief Get recommended page size for a storage device
	[[nodiscard]] size_t GetRecommendedPageSize(const StorageInfo& info) const;

	/// @brief Get recommended cache size for a storage device
	[[nodiscard]] size_t GetRecommendedCacheSize(const StorageInfo& info) const;

	/// @brief Get all available presets
	[[nodiscard]] static std::vector<std::pair<IOBenchmarkPreset, std::string>> GetPresets();

private:
	StorageIOSystem();
	~StorageIOSystem();

	// No copy
	StorageIOSystem(const StorageIOSystem&) = delete;
	StorageIOSystem& operator=(const StorageIOSystem&) = delete;

	// Allow move
	StorageIOSystem(StorageIOSystem&&) noexcept;
	StorageIOSystem& operator=(StorageIOSystem&&) noexcept;

	/// @brief Detect storage devices
	void DetectStorageDevices();

	/// @brief Get storage info for a device
	[[nodiscard]] StorageInfo GetDeviceInfo(const TFilePath& devicePath) const;

	/// @brief Benchmark a specific configuration
	[[nodiscard]] IOBenchmarkResult BenchmarkConfiguration(
		const StorageInfo& info,
		size_t pageSizeInBytes,
		size_t cacheSizeInBytes,
		int numRuns
	) const;

	/// @brief Calculate composite score
	[[nodiscard]] double CalculateScore(const IOBenchmarkResult& result) const;

	/// @brief Get preset configurations
	[[nodiscard]] std::vector<std::pair<size_t, size_t>> GetPresetConfigs(
		IOBenchmarkPreset preset
	) const;

	// State
	bool initialized_ = false;
	std::vector<StorageInfo> storageDevices_;
	mutable std::mutex mutex_;
};

} // namespace hs::system
