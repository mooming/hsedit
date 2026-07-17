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

	[[nodiscard]] auto GetIndex() const { return index; }

	[[nodiscard]] auto& GetLastTimeTouched() const { return lastTimeTouched; }
	[[nodiscard]] auto& GetTextBuffer() { return buffer; }
	[[nodiscard]] auto& GetTextBuffer() const { return buffer; }
	[[nodiscard]] auto GetNumberOfLines() const { return buffer.NumLines(); }

	[[nodiscard]] double GetTimeSinceLastTouch() const;
};
} // hs::system
