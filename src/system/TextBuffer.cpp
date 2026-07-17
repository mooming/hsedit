//
// Created by Hansol Park on 2026. 7. 17..
//

#include "TextBuffer.h"

namespace hs
{
// ========================================================================
// CONSTRUCTORS
// ========================================================================

/// @brief Default constructor: creates empty buffer
TextBuffer::TextBuffer()
{
}

// ========================================================================
// HELPER: Split a line by newlines and append to buffer
// ========================================================================

/// @brief Split a line by newline characters and append to buffer
/// @param dest Destination buffer to append to
/// @param line Input line (may contain newline characters)
static void AppendSplitLines(typename TextBuffer::TLines& dest, const typename TextBuffer::TLine& line)
{
	size_t start = 0;
	size_t pos = line.find(u8'\n');

	while (pos != std::u8string::npos)
	{
		dest.emplace_back(line.data() + start, pos - start);
		start = pos + 1;
		pos = line.find(u8'\n', start);
	}

	dest.emplace_back(line.data() + start, line.size() - start);
}

// ========================================================================
// LINE OPERATIONS
// ========================================================================

/// @brief Append an empty line to the end of the buffer and return reference for modification
/// @return Reference to the newly added line
TextBuffer::TLine& TextBuffer::AddLine()
{
	return lines.emplace_back();
}

/// @brief Append a line to the end of the buffer, splitting by newlines if present
/// @param line Line content (may contain newline characters)
void TextBuffer::AddLine(const TLine& line)
{
	AppendSplitLines(lines, line);
}

/// @brief Append a line using move semantics, splitting by newlines if present
/// @param line Line content (moved, may contain newline characters)
void TextBuffer::EmplaceLine(TLine&& line)
{
	AppendSplitLines(lines, line);
}

/// @brief Insert a line at a specific position, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (may contain newline characters)
void TextBuffer::InsertLine(TLineIndex lineNumber, const TLine& line)
{
	AppendSplitLines(lines, line);
}

/// @brief Insert a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to insert at
/// @param line Line content (moved, may contain newline characters)
void TextBuffer::InsertLine(TLineIndex lineNumber, TLine&& line)
{
	AppendSplitLines(lines, line);
}

/// @brief Replace a line at a specific position with const reference, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (may contain newline characters)
void TextBuffer::ReplaceLine(TLineIndex lineNumber, const TLine& line)
{
	lines.erase(lines.begin() + lineNumber);
	AppendSplitLines(lines, line);
}

/// @brief Replace a line at a specific position using move semantics, splitting by newlines if present
/// @param lineNumber Line index to replace
/// @param line New line content (moved, may contain newline characters)
void TextBuffer::ReplaceLine(TLineIndex lineNumber, TLine&& line)
{
	lines.erase(lines.begin() + lineNumber);
	AppendSplitLines(lines, line);
}

/// @brief Extract and remove a line from the buffer
/// @param lineNumber Line index to extract
/// @return Extracted line content
TextBuffer::TLine TextBuffer::ExtractLine(TLineIndex lineNumber)
{
	TLine extracted = std::move(lines[lineNumber]);
	lines.erase(lines.begin() + lineNumber);
	return extracted;
}

/// @brief Remove a line from the buffer
/// @param lineNumber Line index to remove
void TextBuffer::RemoveLine(TLineIndex lineNumber)
{
	lines.erase(lines.begin() + lineNumber);
}

/// @brief Remove a range of lines from the buffer
/// @param start Start line index (inclusive)
/// @param end End line index (exclusive)
void TextBuffer::RemoveLines(TLineIndex start, TLineIndex end)
{
	lines.erase(lines.begin() + start, lines.begin() + end);
}

// ========================================================================
// SPLIT
// ========================================================================

/// @brief Split the buffer at a specific line
/// @param splitLine Line index to split at
/// @return New buffer containing lines from splitLine onwards
TextBuffer TextBuffer::Split(TLineIndex splitLine)
{
	TextBuffer newBuffer;
	newBuffer.lines.reserve(lines.size() - splitLine);

	// Move lines to new buffer
	for (size_t i = splitLine; i < lines.size(); ++i)
	{
		newBuffer.lines.emplace_back(std::move(lines[i]));
	}

	// Remove moved lines from original buffer
	lines.erase(lines.begin() + splitLine, lines.end());
	return newBuffer;
}

} // namespace hs
