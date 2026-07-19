//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <string>

#include "HSTypes.h"


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

} // namespace hs::system
