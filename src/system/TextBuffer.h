//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <string>
#include <vector>

namespace hs
{

class TextBuffer final
{
public:
	using TLine = std::u8string;
	using TLines = std::vector<TLine>;
	using TLineIndex = size_t;

private:
	TLineIndex startLine;
	TLineIndex hintLineWidth;
	TLines lines;

public:
	TextBuffer();
	TextBuffer(TLineIndex startLine, TLineIndex hintNumLines, TLineIndex hintNumLineWidth);
	~TextBuffer() = default;

	[[nodiscard]] auto& GetBuffer() { return lines; }
	[[nodiscard]] auto& GetBuffer() const { return lines; }

	[[nodiscard]] auto NumLines() const { return lines.size(); }

	TLine& AddLine();

	void AddLine(const TLine& line);
	void EmplaceLine(TLine&& line);
	void InsertLine(TLineIndex lineNumber, const TLine& line);
	void InsertLine(TLineIndex lineNumber, TLine&& line);
	void ReplaceLine(TLineIndex lineNumber, const TLine& line);
	void ReplaceLine(TLineIndex lineNumber, TLine&& line);
	TLine ExtractLine(TLineIndex lineNumber);

	void RemoveLine(TLineIndex lineNumber);
	void RemoveLines(TLineIndex start, TLineIndex end);

	TextBuffer Split(TLineIndex splitLine);
};

} // namespace hs
