//
// Created by mooming on 18/07/2026.
//

#include "StorageIOSystem.h"

namespace hs::system
{

StorageIOSystem::StorageIOSystem()
{
	Initialize();
}

StorageIOSystem::~StorageIOSystem()
{
	Shutdown();
}

void StorageIOSystem::Initialize()
{

}

void StorageIOSystem::Shutdown()
{

}

IOBenchmarkResult StorageIOSystem::IOBenchmark(size_t chunkSize, size_t bufferSize) const
{
	IOBenchmarkResult result;

	result.chunkSie = chunkSize;
	result.bufferSize = bufferSize;

	// Create temporary file for benchmarking
	const TFilePath tempFile(std::filesystem::current_path().string() + "/hsedit_benchmark_temp.bin");
	FILE* file = nullptr;

	// Open timing test
	const auto openStart = std::chrono::steady_clock::now();
	file = std::fopen(tempFile.string().c_str(), "wb+");
	const auto openEnd = std::chrono::steady_clock::now();

	if (file)
	{
		std::fclose(file);
		result.openTimeMs = static_cast<double>(
			std::chrono::duration_cast<std::chrono::microseconds>(openEnd - openStart).count()
		) / 1000.0;
	}

	// Sequential write throughput test
	if (file)
	{
		const size_t numWrites = bufferSize / chunkSize;
		std::vector<char> writeBuffer(chunkSize, 'A');

		const auto writeStart = std::chrono::steady_clock::now();
		for (size_t i = 0; i < numWrites; ++i)
		{
			std::fwrite(writeBuffer.data(), 1, chunkSize, file);
		}
		std::fflush(file);
		const auto writeEnd = std::chrono::steady_clock::now();

		const double writeDurationSec = static_cast<double>(
			std::chrono::duration_cast<std::chrono::microseconds>(writeEnd - writeStart).count()
		) / 1'000'000.0;

		if (writeDurationSec > 0.0)
		{
			result.writeThroughputMBs = (static_cast<double>(bufferSize) / (1024.0 * 1024.0)) / writeDurationSec;
		}
	}

	// Sequential read throughput test
	if (file)
	{
		std::fseek(file, 0, SEEK_SET);
		std::vector<char> readBuffer(chunkSize);

		const auto readStart = std::chrono::steady_clock::now();
		while (std::fread(readBuffer.data(), 1, chunkSize, file) == chunkSize)
		{
			// consume buffer
		}
		const auto readEnd = std::chrono::steady_clock::now();

		const double readDurationSec = static_cast<double>(
			std::chrono::duration_cast<std::chrono::microseconds>(readEnd - readStart).count()
		) / 1'000'000.0;

		if (readDurationSec > 0.0)
		{
			result.readThroughputMBs = (static_cast<double>(bufferSize) / (1024.0 * 1024.0)) / readDurationSec;
		}
	}

	// Random I/O latency test
	if (file)
	{
		const size_t numRandomOps = 100;
		std::vector<size_t> offsets(numRandomOps);
		for (size_t i = 0; i < numRandomOps; ++i)
		{
			offsets[i] = (i * chunkSize * 7) % bufferSize;
		}

		// Random read latency
		{
			std::vector<char> readBuffer(chunkSize);
			const auto randomStart = std::chrono::steady_clock::now();
			for (size_t i = 0; i < numRandomOps; ++i)
			{
				std::fseek(file, static_cast<long>(offsets[i]), SEEK_SET);
				std::fread(readBuffer.data(), 1, chunkSize, file);
			}
			const auto randomEnd = std::chrono::steady_clock::now();

			const double totalUs = static_cast<double>(
				std::chrono::duration_cast<std::chrono::microseconds>(randomEnd - randomStart).count()
			);
			result.randomReadLatencyUs = totalUs / static_cast<double>(numRandomOps);
		}

		// Random write latency
		{
			std::vector<char> writeBuffer(chunkSize, 'A');
			const auto randomStart = std::chrono::steady_clock::now();
			for (size_t i = 0; i < numRandomOps; ++i)
			{
				std::fseek(file, static_cast<long>(offsets[i]), SEEK_SET);
				std::fwrite(writeBuffer.data(), 1, chunkSize, file);
			}
			std::fflush(file);
			const auto randomEnd = std::chrono::steady_clock::now();

			const double totalUs = static_cast<double>(
				std::chrono::duration_cast<std::chrono::microseconds>(randomEnd - randomStart).count()
			);
			result.randomWriteLatencyUs = totalUs / static_cast<double>(numRandomOps);
		}
	}

	// Composite score (lower is better)
	// Weighted combination: 40% write throughput, 30% read throughput, 15% random read, 15% random write
	if (result.writeThroughputMBs > 0.0)
	{
		result.score += 1.0 / result.writeThroughputMBs * 40.0;
	}
	if (result.readThroughputMBs > 0.0)
	{
		result.score += 1.0 / result.readThroughputMBs * 30.0;
	}
	if (result.randomReadLatencyUs > 0.0)
	{
		result.score += result.randomReadLatencyUs * 0.015;
	}
	if (result.randomWriteLatencyUs > 0.0)
	{
		result.score += result.randomWriteLatencyUs * 0.015;
	}

	// Clean up temporary file
	std::remove(tempFile.string().c_str());

	return result;
}

} // namespace hs::system
