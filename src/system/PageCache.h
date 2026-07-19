//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <chrono>

#include "HSTypes.h"
#include "TextBuffer.h"

namespace hs::system
{
class PageCache final
{
private:
	TIndex index;
	TTimestamp lastTimeTouched;
	TextBuffer buffer;

public:
	explicit PageCache(TIndex inIndex);
	~PageCache() = default;

	PageCache(const PageCache&) = default;
	PageCache& operator=(const PageCache&) = default;
	PageCache(PageCache&&) noexcept = default;
	PageCache& operator=(PageCache&&) noexcept = default;

	void Touch();

	/// @brief Get the page index
	[[nodiscard]] TIndex GetIndex() const { return index; }

	/// @brief Get the last time this page was touched
	[[nodiscard]] TTimestamp GetLastTimeTouched() const { return lastTimeTouched; }

	/// @brief Get the text buffer
	[[nodiscard]] auto& GetTextBuffer() { return buffer; }
	[[nodiscard]] auto& GetTextBuffer() const { return buffer; }

	/// @brief Get the number of lines in this page
	[[nodiscard]] TLineIndex GetNumberOfLines() const { return buffer.NumLines(); }

	[[nodiscard]] double GetTimeSinceLastTouch() const;
};

} // namespace hs::system
