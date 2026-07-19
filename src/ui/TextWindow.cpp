//
// Created by Hansol Park on 2026. 7. 17.
//

#include "TextWindow.h"

#include <cassert>


namespace hs::ui
{

TextWindow::TextWindow()
	: hasBorder(true)
	, startPos(LINES >> 2, COLS >> 2)
	, size(LINES >> 1, COLS >> 1)
	, window(nullptr)
{
	window = newwin(size.row, size.column, startPos.row, startPos.column);
	assert(window != nullptr);
}

TextWindow::TextWindow(const Vec2i& startPos, const Vec2i& size)
	: hasBorder(true)
	, startPos(startPos)
	, size(size)
	, window(nullptr)
{
	window = newwin(size.row, size.column, startPos.row, startPos.column);
	assert(window != nullptr);
}

TextWindow::TextWindow(const TextWindow& other)
	: hasBorder(other.hasBorder), startPos(other.startPos), size(other.size), window(nullptr)
{
	if (other.window == nullptr)
		return;

	window = dupwin(other.window);
}

TextWindow::TextWindow(TextWindow&& other) noexcept
	: hasBorder(other.hasBorder)
	, startPos(other.startPos)
	, size(other.size)
	, window(other.window)
{
	other.window = nullptr;
}

TextWindow& TextWindow::operator=(TextWindow&& other) noexcept
{
	if (this != &other)
	{
		hasBorder = other.hasBorder;
		startPos = other.startPos;
		size = other.size;
		window = other.window;
		other.window = nullptr;
	}

	return *this;
}

TextWindow::~TextWindow()
{
	if (window == nullptr)
		return;

	delwin(window);
	window = nullptr;
}

void TextWindow::Refresh()
{
	assert(window != nullptr);

	if (hasBorder)
	{
		wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
	}

	wnoutrefresh(window);
}

void TextWindow::Resize(const Vec2i& newSize)
{
	assert(window != nullptr);
}

void TextWindow::MoveTo(const Vec2i& pos)
{
	assert(window != nullptr);
}

} // hs::ui