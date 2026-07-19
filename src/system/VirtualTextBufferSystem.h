//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <atomic>
#include <thread>

namespace hs::system
{

/// @brief Manages multiple VirtualTextBuffers with background I/O and LRU optimization
/// @details Handles file watching, prefetching, incremental LRU updates, and thread-safe access
///          to multiple buffers. Acts as a facade between the editor and individual buffers.
class VirtualTextBufferSystem final
{
private:
	std::atomic<bool> running;
	std::thread workerThread;

public:
	VirtualTextBufferSystem();
	~VirtualTextBufferSystem();

	// No copy, no move
	VirtualTextBufferSystem(const VirtualTextBufferSystem&) = delete;
	VirtualTextBufferSystem& operator=(const VirtualTextBufferSystem&) = delete;
	VirtualTextBufferSystem(VirtualTextBufferSystem&&) = delete;
	VirtualTextBufferSystem& operator=(VirtualTextBufferSystem&&) = delete;

	void Initialize();
	void Shutdown();
};

} // namespace hs::system
