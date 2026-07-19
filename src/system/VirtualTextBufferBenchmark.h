//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>


namespace hs::system
{

/// @brief Benchmark result for a specific configuration
struct BenchmarkResult
{
	TFilePath configName;
	size_t pageSizeInBytes;
	size_t cacheSizeInBytes;
	
	// Performance metrics (in milliseconds unless noted)
	double openTimeMs;
	double readThroughputMBs;
	double writeThroughputMBs;
	double randomReadLatencyUs;
	size_t memoryUsageBytes;
	
	// Composite score (lower is better)
	double score;
};

/// @brief Benchmark preset configurations
enum class BenchmarkPreset
{
	Mobile,         // Low-memory devices
	Desktop,        // Standard desktop editor
	LargeFile,      // Specialized for large files
	SSDOptimized,   // Optimized for SSD storage
	HDDOptimized,   // Optimized for HDD storage
	AutoDetect      // Auto-detect best configuration
};

/// @brief Benchmark configuration
struct BenchmarkConfig
{
	BenchmarkPreset preset;
	TFilePath testFilePath;
	size_t testFileSizeBytes;  // 0 = use existing file
	int numRuns;  // Number of benchmark runs (average)
	bool verbose;  // Print detailed results
};

/// @brief Benchmark engine for VirtualTextBuffer
class VirtualTextBufferBenchmark final
{
public:
	/// @brief Run benchmark with given configuration
	/// @return Best configuration result
	static BenchmarkResult Run(const BenchmarkConfig& config);

	/// @brief Run benchmark with preset
	/// @return Best configuration result
	static BenchmarkResult RunPreset(BenchmarkPreset preset, const TFilePath& testFilePath);

	/// @brief Get preset configurations
	static std::vector<std::pair<BenchmarkPreset, std::pair<size_t, size_t>>> GetPresets();

private:
	/// @brief Test a specific configuration
	static BenchmarkResult TestConfiguration(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes,
		size_t cacheSizeInBytes,
		int numRuns,
		bool verbose
	);

	/// @brief Measure open time
	static double MeasureOpenTime(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes
	);

	/// @brief Measure sequential read throughput
	static double MeasureSequentialReadThroughput(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes
	);

	/// @brief Measure random read throughput
	static double MeasureRandomReadThroughput(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes
	);

	/// @brief Measure write throughput
	static double MeasureWriteThroughput(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes
	);

	/// @brief Measure random read latency
	static double MeasureRandomReadLatency(
		const TFilePath& testFilePath,
		size_t pageSizeInBytes
	);

	/// @brief Estimate memory usage
	static size_t EstimateMemoryUsage(
		size_t pageSizeInBytes,
		size_t cacheSizeInBytes
	);

	/// @brief Calculate composite score
	static double CalculateScore(
		const BenchmarkResult& result
	);

	/// @brief Query storage information
	static std::pair<bool, size_t> QueryStorageInfo(const TFilePath& path);
};

} // namespace hs::system
