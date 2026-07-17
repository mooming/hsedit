//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <list>
#include <unordered_map>

#include "TextBuffer.h"

namespace hs::system
{
/// @brief Virtualized large text buffer with storage support
/// @details Manages multiple TextBuffer pages with paging and disk persistence.
///          Provides a unified interface as if it were one continuous buffer.
///          Supports LRU cache for page eviction when memory is constrained.
class VirtualTextBuffer final
{
public:
	using TLine = std::u8string;
	using TLineIndex = size_t;
	using PageIndex = size_t;

private:
	/// @brief Page structure for virtualized buffer
	struct Page
	{
		std::unique_ptr<TextBuffer> buffer;
		PageIndex pageId;
		TLineIndex startLine;
		TLineIndex endLine;
		bool modified;
		bool loaded;

		Page();
		Page(PageIndex id, TLineIndex start, TLineIndex end);
	};

	/// @brief Page table - sorted mapping of line ranges to pages
	std::vector<std::unique_ptr<Page>> pages;

	/// @brief Storage path for page files
	std::filesystem::path storagePath;

	/// @brief Whether storage is enabled
	bool storageEnabled;

	/// @brief Next available page ID
	PageIndex nextPageId;

	/// @brief Line count hint for new pages
	TLineIndex pageLineCount;

	/// @brief LRU cache for tracking page access order
	struct LRUNode
	{
		PageIndex pageId;
		std::list<PageIndex>::iterator listIt;
	};

	std::list<PageIndex> lruList;
	std::unordered_map<PageIndex, LRUNode> lruMap;
	PageIndex maxPages;

public:
	/// @brief Constructor
	/// @param storagePath Path to store page files (empty = no persistence)
	/// @param pageLineCount Hint for initial line count per page
	/// @param maxPages Maximum number of pages to keep in memory (0 = unlimited)
	VirtualTextBuffer(const std::filesystem::path& storagePath = "", TLineIndex pageLineCount = 1000, PageIndex maxPages = 0);

	/// @brief Destructor
	~VirtualTextBuffer();

	// No copy
	VirtualTextBuffer(const VirtualTextBuffer&) = delete;
	VirtualTextBuffer& operator=(const VirtualTextBuffer&) = delete;

	// Allow move
	VirtualTextBuffer(VirtualTextBuffer&&) noexcept;
	VirtualTextBuffer& operator=(VirtualTextBuffer&&) noexcept;

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
	[[nodiscard]] std::unique_ptr<VirtualTextBuffer> Split(TLineIndex splitLine) const;

	/// @brief Save all modified pages to storage
	void Save();

	/// @brief Load buffer from storage
	void Load();

	/// @brief Check if a specific page is loaded in memory
	[[nodiscard]] bool IsPageLoaded(PageIndex pageId) const;

	/// @brief Get the number of pages currently in memory
	[[nodiscard]] PageIndex NumLoadedPages() const;

	/// @brief Get the number of pages
	[[nodiscard]] PageIndex NumPages() const;

	/// @brief Get the storage path
	[[nodiscard]] const std::filesystem::path& GetStoragePath() const;

	/// @brief Set maximum number of pages in memory (0 = unlimited)
	void SetMaxPages(PageIndex maxPages);

private:
	/// @brief Find which page contains a given line number
	[[nodiscard]] Page* FindPage(TLineIndex lineNumber) const;

	/// @brief Ensure a page is loaded in memory (with LRU eviction)
	void EnsurePageLoaded(Page* page);

	/// @brief Unload a page from memory (save to disk if modified)
	void UnloadPage(Page* page);

	/// @brief Split a page at a specific line
	void SplitPage(Page* page, TLineIndex splitLine);

	/// @brief Save a page to disk
	void SavePageToDisk(const Page& page) const;

	/// @brief Load a page from disk
	std::unique_ptr<TextBuffer> LoadPageFromDisk(PageIndex pageId, TLineIndex startLine) const;

	/// @brief Generate page file path
	[[nodiscard]] std::filesystem::path GetPageFilePath(PageIndex pageId) const;

	/// @brief Move a page to most recently used position in LRU cache
	void UpdateLRU(PageIndex pageId);

	/// @brief Evict least recently used page if at capacity
	void EvictLRU();
};

} // namespace hs::system
