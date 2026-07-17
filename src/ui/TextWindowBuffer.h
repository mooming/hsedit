//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <string>
#include <vector>

namespace hs::ui
{

class TextWindowBuffer final
{
private:
	size_t startLine;
	size_t hintLineWidth;
	std::vector<std::u8string> lines;

public:
	TextWindowBuffer();
	TextWindowBuffer(size_t startLine, size_t hintNumLines, size_t hintNumLineWidth);
	~TextWindowBuffer() = default;

	[[nodiscard]] auto& GetBuffer() { return lines; }
	[[nodiscard]] auto& GetBuffer() const { return lines; }

	[[nodiscard]] auto NumLines() const { return lines.size(); }

	TextWindowBuffer Split(size_t splitLine);
};

} // namespace hs
