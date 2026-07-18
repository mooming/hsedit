//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

#include "VirtualTextBuffer.h"
#include "HSTypes.h"


namespace hs::system
{

/// @brief Manages multiple VirtualTextBuffers with background I/O and LRU optimization
/// @details Handles file watching, prefetching, incremental LRU updates, and thread-safe access
///          to multiple buffers. Acts as a facade between the editor and individual buffers.
class VirtualTextBufferSystem final
{
public:
	/// @brief Callback for file change notifications
	using FileChangeCallback = std::function<void(const TFilePath& filePath)>;

	/// @brief Callback for load progress notifications
	using LoadProgressCallback = std::function<void(TPageIndex pageId, float progress)>;

	explicit VirtualTextBufferSystem(const std::filesystem::path& baseDir);
	~VirtualTextBufferSystem();

	// No copy
	VirtualTextBufferSystem(const VirtualTextBufferSystem&) = delete;
	VirtualTextBufferSystem& operator=(const VirtualTextBufferSystem&) = delete;

	// Allow move
	VirtualTextBufferSystem(VirtualTextBufferSystem&&) noexcept;
	VirtualTextBufferSystem& operator=(VirtualTextBufferSystem&&) noexcept;

	/// @brief Open a file for editing
	/// @return Handle to the opened buffer (nullptr on failure)
	[[nodiscard]] std::shared_ptr<VirtualTextBuffer> Open(const std::filesystem::path& filePath);

	/// @brief Close a buffer and optionally save changes
	void Close(std::shared_ptr<VirtualTextBuffer> buffer, bool save = true);

	/// @brief Close all open buffers
	void CloseAll(bool save = true);

	/// @brief Check if a buffer is open
	[[nodiscard]] bool IsOpen(const std::filesystem::path& filePath) const;

	/// @brief Get the number of open buffers
	[[nodiscard]] TIndex NumOpenBuffers() const;

	/// @brief Set file change callback
	void SetFileChangeCallback(FileChangeCallback callback);

	/// @brief Set load progress callback
	void SetLoadProgressCallback(LoadProgressCallback callback);

	/// @brief Set cache size hint in bytes (0 = unlimited)
	void SetCacheSize(size_t bytes);

	/// @brief Set initial page size in bytes
	void SetInitialPageSize(size_t bytes);

	/// @brief Set incremental LRU update period (seconds, 0 = disabled)
	void SetIncrementalLRUPeriod(float seconds);

	/// @brief Set LRU life time (seconds)
	void SetLife(float seconds);

	/// @brief Prefetch a page for a specific buffer
	void PrefetchPage(std::shared_ptr<VirtualTextBuffer> buffer, TPageIndex pageId);

	/// @brief Run off-thread updates (call periodically from game loop or worker)
	void Update(float deltaTime);

	/// @brief Stop background threads
	void Stop();

private:
	/// @brief Internal buffer handle
	struct BufferHandle final
	{
		std::shared_ptr<VirtualTextBuffer> buffer;
		std::filesystem::path filePath;
		std::chrono::steady_clock::time_point lastAccess;
		std::chrono::steady_clock::time_point lastModified;
		bool isDirty;
	};

	/// @brief Register a file for watching
	void RegisterFile(const std::filesystem::path& filePath);

	/// @brief Unregister a file from watching
	void UnregisterFile(const std::filesystem::path& filePath);

	/// @brief Check for file changes
	void CheckFileChanges();

	/// @brief Perform incremental LRU update
	void IncrementalLRUUpdate(float deltaTime);

	/// @brief Evict least recently used buffer if over capacity
	void EvictLRU();

	/// @brief Get buffer handle by file path
	[[nodiscard]] BufferHandle* FindHandle(const std::filesystem::path& filePath);

	/// @brief Update last access time for a buffer
	void TouchBuffer(BufferHandle* handle);

	/// @brief Background thread function
	void WorkerThread();

	// Configuration
	size_t cacheSizeInBytes_;
	size_t pageSizeInBytes_;
	float life_;
	float incrementalLRUPeriod_;
	bool running_;

	// State
	std::unordered_map<TFilePath, BufferHandle> openBuffers_;
	std::unordered_map<TFilePath, std::chrono::steady_clock::time_point> fileModTimes_;
	std::vector<TFilePath> filesToCheck_;

	// Callbacks
	FileChangeCallback fileChangeCallback_;
	LoadProgressCallback loadProgressCallback_;

	// Threading
	std::thread workerThread_;
	std::mutex mutex_;
	std::condition_variable condition_;
};

} // namespace hs::system
