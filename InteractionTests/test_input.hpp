// Real (operating system) mouse and keyboard input for nana GUI tests.
// The events go through the OS and nana's event loop, as a user's would: X11 with XTest, Windows with SendInput.
// Call these functions from the test function of nana::exec(wait, wait_end, f), not from the GUI thread.
#pragma once
#include <nana/gui.hpp>

namespace nana_test
{
	enum class keys { down, up, left, right, enter, escape, tab, space };

	/// Moves the pointer to pos, in the coordinates of the window wd.
	void move(nana::window wd, nana::point pos);
	/// Moves the pointer to the center of the window wd.
	void move(nana::window wd);
	/// Clicks (press and release) a mouse button where the pointer is. button: 1 left, 2 middle, 3 right.
	void click(int button = 1);
	/// Moves the pointer to the center of wd and clicks.
	void click(nana::window wd, int button = 1);
	/// Presses and releases a key.
	void key(keys k);
	/// Lets the GUI thread process the events sent so far.
	void settle(unsigned ms = 300);
}
