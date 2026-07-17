//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <ncurses.h>
#include "Vec2i.h"

namespace hs::ui
{

class TextWindow final
{
private:
	bool hasBorder;
	Vec2i startPos;
	Vec2i size;

	WINDOW* window;

public:
	TextWindow();
	TextWindow(const Vec2i& startPos, const Vec2i& size);
	TextWindow(const TextWindow& other);
	~TextWindow();

	void Refresh();

	void Resize(const Vec2i& newSize);
	void MoveTo(const Vec2i& pos);

	auto& GetStartPos() const { return startPos; }
	auto& GetSize() const { return size; }
};

} // namespace hs::ui
