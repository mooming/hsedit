//
// Created by Hansol Park on 2026. 7. 17..
//

#include "TextWindowBuffer.h"

namespace hs::ui
{
TextWindowBuffer::TextWindowBuffer()
	: startLine(0)
	, hintLineWidth(0)
{
}

TextWindowBuffer::TextWindowBuffer(size_t startLine, size_t hintNumLines, size_t hintNumLineWidth)
	: startLine(startLine)
	, hintLineWidth(hintNumLineWidth)
{
	lines.reserve(hintNumLines);
}

} // hs::ui