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
// LINE OPERATIONS
// ========================================================================

/// @brief Append a line to the end of the buffer
/// @param line Line content to add
void TextWindowBuffer::AddLine(const TLine& line)
{
	lines.push_back(line);
}

/// @brief Append a line using move semantics
/// @param line Line content to add (moved)
void TextWindowBuffer::EmplaceLine(TLine&& line)
{
	lines.push_back(std::move(line));
}

/// @brief Insert a line at a specific position
/// @param lineNumber Line index to insert at
/// @param line Line content to insert
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, const TLine& line)
{
	lines.insert(lines.begin() + lineNumber, line);
}

/// @brief Insert a line at a specific position using move semantics
/// @param lineNumber Line index to insert at
/// @param line Line content to insert (moved)
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, TLine&& line)
{
	lines.insert(lines.begin() + lineNumber, std::move(line));
}

/// @brief Replace a line at a specific position with const reference
/// @param lineNumber Line index to replace
/// @param line New line content
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, const TLine& line)
{
	lines[lineNumber] = line;
}

/// @brief Replace a line at a specific position using move semantics
/// @param lineNumber Line index to replace
/// @param line New line content (moved)
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, TLine&& line)
{
	lines[lineNumber] = std::move(line);
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
