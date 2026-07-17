//
// Created by Hansol Park on 2026. 7. 17..
//

#pragma once

#include <ncurses.h>


namespace hs::ui
{

class TextWindow final
{
private:
	WINDOW* window;

public:
	TextWindow();
	~TextWindow();
};

} // namespace hs::ui
