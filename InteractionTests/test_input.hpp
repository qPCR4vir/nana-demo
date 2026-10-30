// Real (operating system) mouse and keyboard input for nana GUI tests.
// The events go through the OS and nana's event loop, as a user's would: X11 with XTest, Windows with SendInput.
// Call these functions from the test function of nana::exec(wait, wait_end, f), not from the GUI thread.
#pragma once
#include <nana/gui.hpp>

namespace nana_test
{
	/// Moves the pointer to pos, in the coordinates of the window wd.
	void move(nana::window wd, nana::point pos);
	/// Moves the pointer to the center of the window wd.
	void move(nana::window wd);
	/// Clicks (press and release) a mouse button where the pointer is. any_button is the left button.
	void click(nana::mouse button = nana::mouse::left_button);
	/// Moves the pointer to the center of wd and clicks.
	void click(nana::window wd, nana::mouse button = nana::mouse::left_button);
	/// Presses and releases a key: a code of nana::keyboard (e.g. keyboard::os_arrow_down, keyboard::enter)
	/// or a character without modifiers (e.g. L'a', L'1').
	void key(wchar_t k);
	/// Lets the GUI thread process the events sent so far.
	void settle(unsigned ms = 300);
}
