//
// Created by Hansol Park on 2026. 7. 17..
//

#include "TextWindowBuffer.h"

namespace hs::ui
{
// ========================================================================
// HELPER FUNCTIONS
// ========================================================================

/// @brief Split a line by newline characters into multiple lines
/// @param line Input line that may contain newline characters
/// @return Vector of lines after splitting
static std::vector<TextWindowBuffer::TLine> SplitByNewline(const TextWindowBuffer::TLine& line)
{
	std::vector<TextWindowBuffer::TLine> result;
	size_t start = 0;
	size_t pos = line.find(u8'\n');

	while (pos != std::u8string::npos)
	{
		result.push_back(line.substr(start, pos - start));
		start = pos + 1;
		pos = line.find(u8'\n', start);
	}

	// Add the remaining part (or the whole line if no newlines)
	result.push_back(line.substr(start));

	return result;
}

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

/// @brief Append a line to the end of the buffer, splitting by newlines if present
/// @param line Line content (may contain newline characters)
void TextWindowBuffer::AddLine(const TLine& line)
{
	auto parts = SplitByNewline(line);
	lines.insert(lines.end(), parts.begin(), parts.end());
}

/// @brief Append a line using move semantics, splitting by newlines if present
/// @param line Line content (moved, may contain newline characters)
void TextWindowBuffer::EmplaceLine(TLine&& line)
{
	auto parts = SplitByNewline(line);
	lines.insert(lines.end(), std::make_move_iterator(parts.begin()), std::make_move_iterator(parts.end()));
}

/// @brief Insert a line at a specific position, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (may contain newline characters)
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, const TLine& line)
{
	auto parts = SplitByNewline(line);
	lines.insert(lines.begin() + lineNumber, parts.begin(), parts.end());
}

/// @brief Insert a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (moved, may contain newline characters)
void TextWindowBuffer::InsertLine(TLineIndex lineNumber, TLine&& line)
{
	auto parts = SplitByNewline(line);
	lines.insert(lines.begin() + lineNumber, std::make_move_iterator(parts.begin()), std::make_move_iterator(parts.end()));
}

/// @brief Replace a line at a specific position with const reference, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (may contain newline characters)
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, const TLine& line)
{
	auto parts = SplitByNewline(line);
	lines.erase(lines.begin() + lineNumber);
	lines.insert(lines.begin() + lineNumber, parts.begin(), parts.end());
}

/// @brief Replace a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (moved, may contain newline characters)
void TextWindowBuffer::ReplaceLine(TLineIndex lineNumber, TLine&& line)
{
	auto parts = SplitByNewline(line);
	lines.erase(lines.begin() + lineNumber);
	lines.insert(lines.begin() + lineNumber, std::make_move_iterator(parts.begin()), std::make_move_iterator(parts.end()));
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
