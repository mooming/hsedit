//
// Created by Hansol Park on 2026. 7. 17..
//

#include "TextWindow.h"

namespace hs::ui
{

TextWindow::TextWindow()
	: window(nullptr)
{

	window = newwin(5, 40, 5, 10);
}

TextWindow::~TextWindow()
{

}
} // hs::ui