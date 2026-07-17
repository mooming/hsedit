//
// Created by Hansol Park on 2026. 7. 17..
//

#include "TextWindowBuffer.h"

namespace hs::ui
{
// ========================================================================
// CONSTRUCTORS
// ========================================================================

/// @brief Default constructor: creates empty buffer starting at line 0
TextWindowBuffer::TextWindowBuffer()
	: startLine(0)
	, hintLineWidth(0)
{
}

/// @brief Parameterized constructor with pre-allocation hints
/// @param startLine Starting line index
/// @param hintNumLines Hint for initial line count
/// @param hintNumLineWidth Hint for initial line width
TextWindowBuffer::TextWindowBuffer(size_t startLine, size_t hintNumLines, size_t hintNumLineWidth)
	: startLine(startLine)
	, hintLineWidth(hintNumLineWidth)
{
	lines.reserve(hintNumLines);
}

// ========================================================================
// HELPER: Split a line by newlines and append to buffer
// ========================================================================

/// @brief Split a line by newline characters and append to buffer
/// @param dest Destination buffer to append to
/// @param line Input line (may contain newline characters)
/// @param moveSource Whether to move from the source string (requires owning string)
static void AppendSplitLines(typename TextWindowBuffer::TLines& dest, const typename TextWindowBuffer::TLine& line, bool moveSource)
{
	size_t start = 0;
	size_t pos = line.find(u8'\n');

	while (pos != std::u8string::npos)
	{
		// Construct directly from pointer and length (no temporary)
		dest.emplace_back(line.data() + start, pos - start);
		start = pos + 1;
		pos = line.find(u8'\n', start);
	}

	// Append the remainder (or entire string if no newlines)
	dest.emplace_back(line.data() + start, line.size() - start);
}

// ========================================================================
// LINE OPERATIONS
// ========================================================================

/// @brief Append a line to the end of the buffer, splitting by newlines if present
/// @param line Line content (may contain newline characters)
void TextWindowBuffer::AddLine(const TLine& line)
{
	AppendSplitLines(lines, line, false);
}

/// @brief Append a line using move semantics, splitting by newlines if present
/// @param line Line content (moved, may contain newline characters)
void TextWindowBuffer::EmplaceLine(TLine&& line)
{
	AppendSplitLines(lines, line, true);
}

/// @brief Insert a line at a specific position, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (may contain newline characters)
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, const TLine& line)
{
	AppendSplitLines(lines, line, false);
}

/// @brief Insert a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (moved, may contain newline characters)
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, TLine&& line)
{
	AppendSplitLines(lines, line, true);
}

/// @brief Replace a line at a specific position with const reference, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (may contain newline characters)
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, const TLine& line)
{
	lines.erase(lines.begin() + lineNumber);
	AppendSplitLines(lines, line, false);
}

/// @brief Replace a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (moved, may contain newline characters)
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, TLine&& line)
{
	lines.erase(lines.begin() + lineNumber);
	AppendSplitLines(lines, line, true);
}

/// @brief Extract and remove a line from the buffer
/// @param lineNumber Line index to extract
/// @return Extracted line content
TextWindowBuffer::TLine TextWindowBuffer::ExtractLine(TLineIndex lineNumber)
{
	TLine extracted = std::move(lines[lineNumber]);
	lines.erase(lines.begin() + lineNumber);
	return extracted;
}

/// @brief Remove a line from the buffer
/// @param lineNumber Line index to remove
void TextWindowBuffer::RemoveLine(TLineIndex lineNumber)
{
	lines.erase(lines.begin() + lineNumber);
}

// ========================================================================
// SPLIT
// ========================================================================

/// @brief Split the buffer at a specific line
/// @param splitLine Line index to split at
/// @return New buffer containing lines from splitLine onwards
TextWindowBuffer TextWindowBuffer::Split(TLineIndex splitLine)
{
	TextWindowBuffer newBuffer(startLine + splitLine, 0, hintLineWidth);
	newBuffer.lines.insert(newBuffer.lines.end(), lines.begin() + splitLine, lines.end());
	lines.erase(lines.begin() + splitLine, lines.end());
	return newBuffer;
}

} // namespace hs::ui
