//
// Created by Hansol Park on 2026. 7. 17.
//

#include "TextApplication.h"

#include <ncurses.h>


namespace hs::ui
{

TextApplication::TextApplication()
{
	initscr();
}

TextApplication::~TextApplication()
{
	endwin();
}

void TextApplication::EndFrame()
{
	doupdate();
}

} // namespace hs::ui