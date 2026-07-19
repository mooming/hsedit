//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <cstddef>


namespace hs::system
{

/// @brief Benchmark result for a specific I/O configuration
struct IOBenchmarkResult final
{
	// Read/Write block size for benchmark tests
	size_t chunkSie = 0;

	// Memory buffer size for benchmark tests
	size_t bufferSize = 0;

	// Performance metrics
	double openTimeMs = 0.0;
	double readThroughputMBs = 0.0;
	double writeThroughputMBs = 0.0;
	double randomReadLatencyUs = 0.0;
	double randomWriteLatencyUs = 0.0;
	size_t memoryUsageBytes = 0;

	// Composite score (lower is better)
	double score = 0.0;
};

} // namespace hs::system
