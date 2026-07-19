//
// Created by mooming on 18/07/2026.
//

#include "PageCache.h"


namespace hs::system
{

PageCache::PageCache(TIndex inIndex)
	: index(inIndex)
	, lastTimeTouched(std::chrono::steady_clock::now())
{
}

void PageCache::Touch()
{
	lastTimeTouched = std::chrono::steady_clock::now();
}

[[nodiscard]] double PageCache::GetTimeSinceLastTouch() const
{
	const auto now = std::chrono::steady_clock::now();
	const auto duration = now - lastTimeTouched;
	return std::chrono::duration<double>(duration).count();
}

} // namespace hs::system