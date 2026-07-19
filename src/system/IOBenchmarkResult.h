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
	size_t chunkSie;

	// Memory buffer size for benchmark tests
	size_t bufferSize;

	// Performance metrics
	double openTimeMs;             // Time to open a file (ms)
	double readThroughputMBs;      // Sequential read throughput (MB/s)
	double writeThroughputMBs;     // Sequential write throughput (MB/s)
	double randomReadLatencyUs;    // Random read latency (us)
	double randomWriteLatencyUs;   // Random write latency (us)
	size_t memoryUsageBytes;       // Estimated memory usage

	// Composite score (lower is better)
	double score;
};

} // namespace hs::system
