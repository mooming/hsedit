//
// Created by Hansol Park on 2026. 7. 17.
//

#pragma once


namespace hs::ui
{

class TextApplication final
{
public:
	TextApplication();
	~TextApplication();

	/// @brief Copy window buffer to stdscr
	void EndFrame();
};

} // namespace hs::ui
