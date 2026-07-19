//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <memory>
#include <filesystem>

#include "PageCache.h"
#include "HSTypes.h"
#include "TextBuffer.h"


namespace hs::system
{
/// @brief Virtualized large text buffer with storage support
/// @details Manages multiple TextBuffer pages with paging and disk persistence.
///          Provides a unified interface as if it were one continuous buffer.
///          Supports LRU cache for page eviction when memory is constrained.
class VirtualTextBuffer final
{
private:
	struct
	{
		// Cache Policy

		// duration for incremental LRU eviction (seconds)
		float life;

		// Time interval for incremental LRU update (seconds)
		float incrementalLRUPeriod;

		// Cache size hint in bytes
		size_t cacheSizeInBytes;

		// Initial page size in bytes. Page will be halved if its size exceeds double of this value.
		size_t pageSizeInBytes;
	};

	TFilePath baseFilePath;

	// Structure of Arrays
	struct
	{
		std::vector<TIndex> pageStarts;
		std::vector<TIndex> pageEnds;
		std::vector<TLineIndex> pageLineStart;
		std::vector<TLineIndex> pageNumLines;
		std::vector<TFilePath> pageFiles;
	};

	std::vector<PageCache> buffer;

public:
	/// @brief Constructor
	VirtualTextBuffer();
	explicit VirtualTextBuffer(const TFilePath& filePath);

	/// @brief Destructor
	~VirtualTextBuffer();

	// No copy
	VirtualTextBuffer(const VirtualTextBuffer&) = delete;
	VirtualTextBuffer& operator=(const VirtualTextBuffer&) = delete;

	// Allow move
	VirtualTextBuffer(VirtualTextBuffer&&) noexcept;
	VirtualTextBuffer& operator=(VirtualTextBuffer&&) noexcept;

	/// @brief Create a virtual table for the given file.
	void Open(const TFilePath& filePath);

	/// @brief Collapse all pages into the single base file.
	void Close();

	/// @brief off-thread update function. It should be called by VirtualTextBufferSystem.
	void Update_Async(float deltaTime);

	/// @brief Get total number of lines across all pages
	[[nodiscard]] TLineIndex NumLines() const;

	/// @brief Add a line at the end of the buffer
	void AddLine(const TLine& line);

	/// @brief Add a line at a specific position
	void InsertLine(TLineIndex lineNumber, const TLine& line);

	/// @brief Replace a line at a specific position
	void ReplaceLine(TLineIndex lineNumber, const TLine& line);

	/// @brief Extract and remove a line
	TLine ExtractLine(TLineIndex lineNumber);

	/// @brief Remove a line
	void RemoveLine(TLineIndex lineNumber);

	/// @brief Get a line by index (loads page if needed)
	[[nodiscard]] const TLine& GetLine(TLineIndex lineNumber) const;

	/// @brief Split the buffer at a line boundary
	[[nodiscard]] TPageIndex Split(TPageIndex pageIndex, TLineIndex splitLine) const;

	/// @brief Check if a specific page is loaded in memory
	[[nodiscard]] bool IsPageLoaded(TPageIndex pageId) const;

	/// @brief Get the number of pages currently in memory
	[[nodiscard]] TPageIndex NumLoadedPages() const;

	/// @brief Get the number of pages
	[[nodiscard]] TPageIndex NumPages() const;

	/// @brief Get the storage path
	[[nodiscard]] const TFilePath& GetStoragePath() const { return baseFilePath; }

private:
	/// @brief Find which page contains a given line number
	[[nodiscard]] TPageIndex FindPage(TLineIndex lineNumber) const;

	/// @brief Ensure a page is loaded in memory (with LRU eviction)
	void EnsurePageLoaded(TPageIndex PageIndex);

	/// @brief Unload a page from memory (save to disk if modified)
	void UnloadPage(TPageIndex PageIndex);

	/// @brief Split a page at a specific line
	void SplitPage(TPageIndex PageIndex, TLineIndex splitLine);

	/// @brief Save a page to disk
	void SavePageToDisk(TPageIndex PageIndex) const;

	void Prefetch(TPageIndex PageIndex);

	/// @brief Load a page from disk
	void LoadPageFromDisk(TPageIndex pageIndex, TLineIndex startLine) const;

	/// @brief Generate page file path
	[[nodiscard]] TFilePath GetPageFilePath(TPageIndex pageIndex) const;

	/// @brief Move a page to most recently used position in LRU cache
	void Touch(TPageIndex pageIndex);

	/// @brief Evict least recently used page if at capacity
	void EvictLRU();
};

} // namespace hs::system
