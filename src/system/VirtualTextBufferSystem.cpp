//
// Created by mooming on 18/07/2026.
//

#include "VirtualTextBufferSystem.h"

#include <algorithm>
#include <iostream>


namespace hs::system
{

VirtualTextBufferSystem::VirtualTextBufferSystem(const std::filesystem::path& baseDir)
	: cacheSizeInBytes_(0)
	, pageSizeInBytes_(0)
	, life_(0.0f)
	, incrementalLRUPeriod_(0.0f)
	, running_(false)
{
	(void)baseDir;
}

VirtualTextBufferSystem::~VirtualTextBufferSystem()
{
	Stop();
	CloseAll(false);
}

VirtualTextBufferSystem::VirtualTextBufferSystem(VirtualTextBufferSystem&&) noexcept = default;

VirtualTextBufferSystem& VirtualTextBufferSystem::operator=(VirtualTextBufferSystem&&) noexcept = default;

std::shared_ptr<VirtualTextBuffer> VirtualTextBufferSystem::Open(const std::filesystem::path& filePath)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (openBuffers_.contains(filePath))
	{
		return openBuffers_[filePath].buffer;
	}

	auto buffer = std::make_shared<VirtualTextBuffer>(filePath);
	buffer->Open();

	BufferHandle handle;
	handle.buffer = buffer;
	handle.filePath = filePath;
	handle.lastAccess = std::chrono::steady_clock::now();
	handle.lastModified = std::chrono::file_time_of<std::chrono::steady_clock>::from_seq(std::filesystem::file_time::epoch());
	handle.isDirty = false;

	openBuffers_[filePath] = std::move(handle);

	RegisterFile(filePath);

	return buffer;
}

void VirtualTextBufferSystem::Close(std::shared_ptr<VirtualTextBuffer> buffer, bool save)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (!buffer)
	{
		return;
	}

	const auto filePath = buffer->GetStoragePath();

	if (openBuffers_.contains(filePath))
	{
		auto& handle = openBuffers_[filePath];

		if (save && handle.isDirty)
		{
			handle.buffer->Close();
		}
		else
		{
			handle.buffer->Close();
		}

		UnregisterFile(filePath);
		openBuffers_.erase(filePath);
	}
}

void VirtualTextBufferSystem::CloseAll(bool save)
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto& [filePath, handle] : openBuffers_)
	{
		if (save && handle.isDirty)
		{
			handle.buffer->Close();
		}
		else
		{
			handle.buffer->Close();
		}

		UnregisterFile(filePath);
	}

	openBuffers_.clear();
}

bool VirtualTextBufferSystem::IsOpen(const std::filesystem::path& filePath) const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return openBuffers_.contains(filePath);
}

TIndex VirtualTextBufferSystem::NumOpenBuffers() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return openBuffers_.size();
}

void VirtualTextBufferSystem::SetFileChangeCallback(FileChangeCallback callback)
{
	std::lock_guard<std::mutex> lock(mutex_);
	fileChangeCallback_ = std::move(callback);
}

void VirtualTextBufferSystem::SetLoadProgressCallback(LoadProgressCallback callback)
{
	std::lock_guard<std::mutex> lock(mutex_);
	loadProgressCallback_ = std::move(callback);
}

void VirtualTextBufferSystem::SetCacheSize(size_t bytes)
{
	std::lock_guard<std::mutex> lock(mutex_);
	cacheSizeInBytes_ = bytes;
}

void VirtualTextBufferSystem::SetInitialPageSize(size_t bytes)
{
	std::lock_guard<std::mutex> lock(mutex_);
	pageSizeInBytes_ = bytes;
}

void VirtualTextBufferSystem::SetIncrementalLRUPeriod(float seconds)
{
	std::lock_guard<std::mutex> lock(mutex_);
	incrementalLRUPeriod_ = seconds;
}

void VirtualTextBufferSystem::SetLife(float seconds)
{
	std::lock_guard<std::mutex> lock(mutex_);
	life_ = seconds;
}

void VirtualTextBufferSystem::PrefetchPage(std::shared_ptr<VirtualTextBuffer> buffer, TPageIndex pageId)
{
	if (!buffer)
	{
		return;
	}

	buffer->Prefetch(pageId);
}

void VirtualTextBufferSystem::Update(float deltaTime)
{
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (!running_)
		{
			running_ = true;
		}

		CheckFileChanges();
		IncrementalLRUUpdate(deltaTime);
	}

	condition_.notify_one();
}

void VirtualTextBufferSystem::Stop()
{
	{
		std::lock_guard<std::mutex> lock(mutex_);
		running_ = false;
	}

	condition_.notify_one();

	if (workerThread_.joinable())
	{
		workerThread_.join();
	}
}

void VirtualTextBufferSystem::RegisterFile(const std::filesystem::path& filePath)
{
	(void)filePath;
	// TODO: Implement file watching (inotify on Linux, FSEvents on macOS, ReadDirectoryChangesW on Windows)
}

void VirtualTextBufferSystem::UnregisterFile(const std::filesystem::path& filePath)
{
	(void)filePath;
	// TODO: Remove from file watcher
}

void VirtualTextBufferSystem::CheckFileChanges()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (const auto& filePath : filesToCheck_)
	{
		if (!std::filesystem::exists(filePath))
		{
			continue;
		}

		const auto currentTime = std::chrono::file_time_of<std::chrono::steady_clock>::from_seq(std::filesystem::file_time::epoch());
		const auto fileTime = std::filesystem::last_write_time(filePath);

		if (fileModTimes_.contains(filePath))
		{
			if (fileTime != fileModTimes_[filePath])
			{
				// File changed externally
				if (fileChangeCallback_)
				{
					fileChangeCallback_(filePath);
				}
			}
		}

		fileModTimes_[filePath] = fileTime;
	}

	filesToCheck_.clear();
}

void VirtualTextBufferSystem::IncrementalLRUUpdate(float deltaTime)
{
	if (incrementalLRUPeriod_ <= 0.0f)
	{
		return;
	}

	// TODO: Implement incremental LRU update
	// - Track time since last LRU update
	// - When elapsed time >= incrementalLRUPeriod_, perform LRU sweep
	// - Evict buffers that haven't been accessed for >= life_ seconds
	// - Respect cacheSizeInBytes_ limit
}

void VirtualTextBufferSystem::EvictLRU()
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (openBuffers_.empty())
	{
		return;
	}

	// Find least recently used buffer
	auto lruIt = std::min_element(
		openBuffers_.begin(),
		openBuffers_.end(),
		[](const auto& a, const auto& b) {
			return a.second.lastAccess < b.second.lastAccess;
		}
	);

	if (lruIt == openBuffers_.end())
	{
		return;
	}

	// Check if we're over cache size limit
	if (cacheSizeInBytes_ > 0)
	{
		size_t totalSize = 0;
		for (const auto& [filePath, handle] : openBuffers_)
		{
			totalSize += handle.buffer->GetStoragePath().string().size();
		}

		if (totalSize <= cacheSizeInBytes_)
		{
			return;
		}
	}

	// Evict LRU buffer
	const auto& lruPath = lruIt->first;
	auto& lruHandle = lruIt->second;

	if (lruHandle.isDirty)
	{
		lruHandle.buffer->Close();
	}
	else
	{
		lruHandle.buffer->Close();
	}

	UnregisterFile(lruPath);
	openBuffers_.erase(lruPath);
}

VirtualTextBufferSystem::BufferHandle* VirtualTextBufferSystem::FindHandle(const std::filesystem::path& filePath)
{
	auto it = openBuffers_.find(filePath);
	if (it != openBuffers_.end())
	{
		return &it->second;
	}

	return nullptr;
}

void VirtualTextBufferSystem::TouchBuffer(BufferHandle* handle)
{
	if (handle)
	{
		handle->lastAccess = std::chrono::steady_clock::now();
	}
}

void VirtualTextBufferSystem::WorkerThread()
{
	while (running_)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		condition_.wait_for(lock, std::chrono::milliseconds(100));

		if (!running_)
		{
			break;
		}

		CheckFileChanges();
		IncrementalLRUUpdate(0.1f);
	}
}

} // namespace hs::system
