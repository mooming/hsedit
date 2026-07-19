//
// Created by mooming on 18/07/2026.
//

#include "StorageIOSystem.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <filesystem>


namespace hs::system
{

StorageIOSystem::StorageIOSystem()
	: initialized_(false)
{
}

StorageIOSystem::~StorageIOSystem() = default;

StorageIOSystem::StorageIOSystem(StorageIOSystem&&) noexcept = default;

StorageIOSystem& StorageIOSystem::operator=(StorageIOSystem&&) noexcept = default;

void StorageIOSystem::Initialize()
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	if (initialized_)
	{
		return;
	}
	
	DetectStorageDevices();
	initialized_ = true;
}

void StorageIOSystem::Shutdown()
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	if (!initialized_)
	{
		return;
	}
	
	storageDevices_.clear();
	initialized_ = false;
}

std::vector<StorageInfo> StorageIOSystem::GetStorageDevices() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return storageDevices_;
}

StorageInfo StorageIOSystem::GetStorageInfoForPath(const TFilePath& path) const
{
	std::lock_guard<std::mutex> lock(mutex_);
	
	const auto& parentPath = path.parent_path();
	
	for (const auto& device : storageDevices_)
	{
		if (device.mountPoint == parentPath || parentPath.starts_with(device.mountPoint))
		{
			return device;
		}
	}
	
	// Return first device as fallback
	if (!storageDevices_.empty())
	{
		return storageDevices_[0];
	}
	
	// Return default info
	return StorageInfo{};
}

IOBenchmarkResult StorageIOSystem::IOBenchmark(
	IOBenchmarkPreset preset,
	const TFilePath& testFilePath,
	size_t testFileSizeBytes,
	int numRuns
) const
{
	Initialize();
	
	// Get storage info for the test path
	TFilePath basePath = testFilePath.empty() ? std::filesystem::temp_directory_path() : testFilePath.parent_path();
	StorageInfo storageInfo = GetStorageInfoForPath(basePath);
	
	// Get preset configurations
	auto configs = GetPresetConfigs(preset);
	
	IOBenchmarkResult bestResult;
	double bestScore = std::numeric_limits<double>::max();
	
	for (const auto& [pageSize, cacheSize] : configs)
	{
		auto result = BenchmarkConfiguration(storageInfo, pageSize, cacheSize, numRuns);
		
		if (result.score < bestScore)
		{
			bestScore = result.score;
			bestResult = result;
		}
	}
	
	return bestResult;
}

size_t StorageIOSystem::GetRecommendedPageSize(const StorageInfo& info) const
{
	if (info.isSSD)
	{
		// SSDs benefit from larger I/O
		return std::max(info.optimalIOSizeBytes, static_cast<size_t>(256 * 1024));
	}
	else
	{
		// HDDs benefit from even larger pages (reduce seeks)
		return std::max(info.optimalIOSizeBytes, static_cast<size_t>(1024 * 1024));
	}
}

size_t StorageIOSystem::GetRecommendedCacheSize(const StorageInfo& info) const
{
	// Target: cache at least 100-1000 pages
	const auto targetPages = info.isSSD ? 256 : 128;
	
	// But respect memory limits (cap at 512MB)
	const auto maxCache = static_cast<size_t>(512 * 1024 * 1024);
	
	return std::min(info.optimalIOSizeBytes * targetPages, maxCache);
}

std::vector<std::pair<IOBenchmarkPreset, std::string>> StorageIOSystem::GetPresets()
{
	return {
		{IOBenchmarkPreset::Mobile, "Mobile (low memory)"},
		{IOBenchmarkPreset::Desktop, "Desktop"},
		{IOBenchmarkPreset::LargeFile, "Large File"},
		{IOBenchmarkPreset::SSDOptimized, "SSD Optimized"},
		{IOBenchmarkPreset::HDDOptimized, "HDD Optimized"},
		{IOBenchmarkPreset::AutoDetect, "Auto Detect"}
	};
}

void StorageIOSystem::DetectStorageDevices()
{
	// Implementation depends on platform
	// For now, return a basic storage info
	
	StorageInfo defaultInfo;
	defaultInfo.devicePath = "/dev/disk0";
	defaultInfo.mountPoint = "/";
	defaultInfo.deviceName = "Default Storage";
	defaultInfo.deviceType = "SSD";
	defaultInfo.isSSD = true;
	defaultInfo.isRemovable = false;
	defaultInfo.totalSizeBytes = 500 * 1024ULL * 1024 * 1024;  // 500GB
	defaultInfo.freeSizeBytes = 250 * 1024ULL * 1024 * 1024;   // 250GB
	defaultInfo.optimalIOSizeBytes = 256 * 1024;  // 256KB
	defaultInfo.maxSequentialReadMBs = 3000;      // 3GB/s
	defaultInfo.maxSequentialWriteMBs = 2500;     // 2.5GB/s
	defaultInfo.randomReadIOPS = 500000;
	defaultInfo.randomWriteIOPS = 100000;
	defaultInfo.percentageUsed = 0.1;
	defaultInfo.fileSystemType = "APFS";
	defaultInfo.blockSizeBytes = 4096;
	
	storageDevices_.push_back(defaultInfo);
}

StorageInfo StorageIOSystem::GetDeviceInfo(const TFilePath& devicePath) const
{
	// Implementation depends on platform
	return StorageInfo{};
}

IOBenchmarkResult StorageIOSystem::BenchmarkConfiguration(
	const StorageInfo& info,
	size_t pageSizeInBytes,
	size_t cacheSizeInBytes,
	int numRuns
) const
{
	IOBenchmarkResult result;
	result.pageSizeInBytes = pageSizeInBytes;
	result.cacheSizeInBytes = cacheSizeInBytes;
	
	// Create temp file for benchmarking
	TFilePath testFile = std::filesystem::temp_directory_path() / "hsedit_benchmark_test.txt";
	
	// Generate test data
	std::vector<char> testData(pageSizeInBytes);
	std::mt19937 gen(42);
	std::uniform_int_distribution<int> dist(0, 255);
	for (auto& byte : testData)
	{
		byte = static_cast<char>(dist(gen));
	}
	
	// Benchmark open time
	double totalOpenTime = 0;
	for (int i = 0; i < numRuns; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		// Simulate file open
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		
		auto end = std::chrono::high_resolution_clock::now();
		totalOpenTime += std::chrono::duration<double, std::milli>(end - start).count();
	}
	result.openTimeMs = totalOpenTime / numRuns;
	
	// Benchmark read throughput
	double totalReadThroughput = 0;
	for (int i = 0; i < numRuns; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		// Simulate sequential read
		std::this_thread::sleep_for(std::chrono::microseconds(500));
		
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(end - start).count();
		double throughput = (pageSizeInBytes / (1024.0 * 1024.0)) / elapsed;
		totalReadThroughput += throughput;
	}
	result.readThroughputMBs = totalReadThroughput / numRuns;
	
	// Benchmark write throughput
	double totalWriteThroughput = 0;
	for (int i = 0; i < numRuns; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		// Simulate sequential write
		std::this_thread::sleep_for(std::chrono::microseconds(600));
		
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(end - start).count();
		double throughput = (pageSizeInBytes / (1024.0 * 1024.0)) / elapsed;
		totalWriteThroughput += throughput;
	}
	result.writeThroughputMBs = totalWriteThroughput / numRuns;
	
	// Benchmark random read latency
	double totalRandomReadLatency = 0;
	for (int i = 0; i < numRuns; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		// Simulate random read
		std::this_thread::sleep_for(std::chrono::microseconds(50));
		
		auto end = std::chrono::high_resolution_clock::now();
		totalRandomReadLatency += std::chrono::duration<double, std::micro>(end - start).count();
	}
	result.randomReadLatencyUs = totalRandomReadLatency / numRuns;
	
	// Benchmark random write latency
	double totalRandomWriteLatency = 0;
	for (int i = 0; i < numRuns; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();
		
		// Simulate random write
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		
		auto end = std::chrono::high_resolution_clock::now();
		totalRandomWriteLatency += std::chrono::duration<double, std::micro>(end - start).count();
	}
	result.randomWriteLatencyUs = totalRandomWriteLatency / numRuns;
	
	// Estimate memory usage
	result.memoryUsageBytes = cacheSizeInBytes + (pageSizeInBytes * 10);  // Cache + overhead
	
	// Calculate score (lower is better)
	result.score = CalculateScore(result);
	
	// Generate recommendation
	if (result.score < 100)
	{
		result.recommendation = "Excellent configuration";
	}
	else if (result.score < 200)
	{
		result.recommendation = "Good configuration";
	}
	else
	{
		result.recommendation = "Suboptimal configuration - consider adjusting";
	}
	
	return result;
}

double StorageIOSystem::CalculateScore(const IOBenchmarkResult& result) const
{
	// Weighted score: lower is better
	// Weights: open time (0.2), read throughput (0.3), write throughput (0.3), latency (0.2)
	
	const auto openScore = result.openTimeMs * 10;  // Normalize
	const auto readScore = 1000.0 / std::max(result.readThroughputMBs, 1.0);
	const auto writeScore = 1000.0 / std::max(result.writeThroughputMBs, 1.0);
	const auto latencyScore = (result.randomReadLatencyUs + result.randomWriteLatencyUs) * 0.01;
	
	return openScore * 0.2 + readScore * 0.3 + writeScore * 0.3 + latencyScore * 0.2;
}

std::vector<std::pair<size_t, size_t>> StorageIOSystem::GetPresetConfigs(
	IOBenchmarkPreset preset
) const
{
	std::vector<std::pair<size_t, size_t>> configs;
	
	switch (preset)
	{
	case IOBenchmarkPreset::Mobile:
		configs = {
			{128 * 1024, 32 * 1024 * 1024},
			{256 * 1024, 64 * 1024 * 1024},
			{512 * 1024, 128 * 1024 * 1024}
		};
		break;
		
	case IOBenchmarkPreset::Desktop:
		configs = {
			{256 * 1024, 128 * 1024 * 1024},
			{512 * 1024, 256 * 1024 * 1024},
			{1024 * 1024, 256 * 1024 * 1024}
		};
		break;
		
	case IOBenchmarkPreset::LargeFile:
		configs = {
			{512 * 1024, 256 * 1024 * 1024},
			{1024 * 1024, 512 * 1024 * 1024},
			{2048 * 1024, 512 * 1024 * 1024}
		};
		break;
		
	case IOBenchmarkPreset::SSDOptimized:
		configs = {
			{256 * 1024, 256 * 1024 * 1024},
			{512 * 1024, 512 * 1024 * 1024},
			{1024 * 1024, 512 * 1024 * 1024}
		};
		break;
		
	case IOBenchmarkPreset::HDDOptimized:
		configs = {
			{512 * 1024, 128 * 1024 * 1024},
			{1024 * 1024, 256 * 1024 * 1024},
			{2048 * 1024, 256 * 1024 * 1024}
		};
		break;
		
	case IOBenchmarkPreset::AutoDetect:
		// Will be overridden by storage info
		configs = {
			{256 * 1024, 128 * 1024 * 1024},
			{512 * 1024, 256 * 1024 * 1024},
			{1024 * 1024, 256 * 1024 * 1024}
		};
		break;
	}
	
	return configs;
}

void StorageIOSystem::Update(float deltaTime)
{
	// TODO: Implement periodic tasks such as:
	// - File change detection
	// - LRU cache updates
	// - Prefetch scheduling
	// - Wear leveling estimation updates
	
	accumulator_ += deltaTime;
	
	// Example: Check for file changes every 1 second
	if (accumulator_ >= 1.0f)
	{
		accumulator_ -= 1.0f;
		// CheckFileChanges();
	}
}

} // namespace hs::system
