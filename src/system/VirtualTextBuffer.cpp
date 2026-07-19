//
// Created by mooming on 18/07/2026.
//

#include "VirtualTextBuffer.h"


namespace hs::system
{

namespace
{

TFilePath GeneratePageFilePath(const TFilePath& baseFilePath, TPageIndex pageIndex)
{
	return baseFilePath.parent_path() / (baseFilePath.stem().string() + "_page_" + std::to_string(pageIndex) + ".txt");
}

} // namespace


VirtualTextBuffer::VirtualTextBuffer()
	: life(0.0f)
	, incrementalLRUPeriod(0.0f)
	, cacheSizeInBytes(0)
	, pageSizeInBytes(4096)
{
}

VirtualTextBuffer::VirtualTextBuffer(const TFilePath& filePath)
	: life(0.0f)
	, incrementalLRUPeriod(0.0f)
	, cacheSizeInBytes(64000000)
	, pageSizeInBytes(4096)
	, baseFilePath(filePath)
{
}

VirtualTextBuffer::~VirtualTextBuffer() = default;

VirtualTextBuffer::VirtualTextBuffer(VirtualTextBuffer&&) noexcept = default;

VirtualTextBuffer& VirtualTextBuffer::operator=(VirtualTextBuffer&&) noexcept = default;

void VirtualTextBuffer::Open(const TFilePath& filePath)
{
	baseFilePath = filePath;

	// 1. open file path
	// 2. read by pages

	// Fill (pageStarts, pageEnds) and (pageLineStart, pageNumLines)
	// Build pageFiles (file name)
}

void VirtualTextBuffer::Close()
{
	// Merge cache files and collapse into the base file.
	// Iterate each cache file to build up a new temp file first. Prefetch next cache file to optimise performance.
	// After building a full copy of the temp file (in memory or in storage), swap the original base file content.
}

void VirtualTextBuffer::Update_Async(float deltaTime)
{

}

TLineIndex VirtualTextBuffer::NumLines() const
{
	return 0;
}

void VirtualTextBuffer::AddLine(const TLine& line)
{

}

void VirtualTextBuffer::InsertLine(TLineIndex lineNumber, const TLine& line)
{

}

void VirtualTextBuffer::ReplaceLine(TLineIndex lineNumber, const TLine& line)
{

}

TLine VirtualTextBuffer::ExtractLine(TLineIndex lineNumber)
{
	return 0;
}

void VirtualTextBuffer::RemoveLine(TLineIndex lineNumber)
{

}

const TLine& VirtualTextBuffer::GetLine(TLineIndex lineNumber) const
{
	static const TLine emptyLine;

	return emptyLine;
}

TPageIndex VirtualTextBuffer::Split(TPageIndex pageIndex, TLineIndex splitLine) const
{
	return 0;
}

bool VirtualTextBuffer::IsPageLoaded(TPageIndex pageId) const
{
	return false;
}

TPageIndex VirtualTextBuffer::NumLoadedPages() const
{
	return 0;
}

TPageIndex VirtualTextBuffer::NumPages() const
{
	return 0;
}

TPageIndex VirtualTextBuffer::FindPage(TLineIndex lineNumber) const
{
	return 0;
}

void VirtualTextBuffer::EnsurePageLoaded(TPageIndex pageIndex)
{

}

void VirtualTextBuffer::UnloadPage(TPageIndex pageIndex)
{

}

void VirtualTextBuffer::SplitPage(TPageIndex pageIndex, TLineIndex splitLine)
{

}

void VirtualTextBuffer::SavePageToDisk(TPageIndex pageIndex) const
{

}

void VirtualTextBuffer::Prefetch(TPageIndex pageIndex)
{

}

void VirtualTextBuffer::LoadPageFromDisk(TPageIndex pageIndex, TLineIndex startLine) const
{

}

TFilePath VirtualTextBuffer::GetPageFilePath(TPageIndex pageIndex) const
{
	return TFilePath{};
}

void VirtualTextBuffer::Touch(TPageIndex pageIndex)
{

}

void VirtualTextBuffer::EvictLRU()
{

}

} // namespace hs::system
