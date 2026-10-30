// Real (operating system) mouse and keyboard input for nana GUI tests.
// The events go through the OS and nana's event loop, as a user's would: X11 with XTest, Windows with SendInput.
// Call these functions from the test function of nana::exec(wait, wait_end, f), not from the GUI thread.
#pragma once
#include <nana/gui.hpp>

/// Real input for nana GUI tests (see InteractionTests/README.md).
/// Before every input event the helper checks that the input goes to a window of this test. If not (on Windows: a
/// window of another program is in the foreground and this one cannot take its place), it sends nothing more,
/// prints why and ends the program with skip_code: input must never reach another program.
namespace nana_test
{
	/// Exit code of a test that could not send its input: CTest reports the test as skipped (SKIP_RETURN_CODE).
	constexpr int skip_code = 77;

	/// Moves the pointer to pos, in the coordinates of the window wd.
	void move(nana::window wd, nana::point pos);
	/// Moves the pointer to the center of the window wd.
	void move(nana::window wd);
	/// Clicks (press and release) a mouse button where the pointer is. any_button is the left button.
	void click(nana::mouse button = nana::mouse::left_button);
	/// Moves the pointer onto wd and then to its center (so that wd sees the pointer move in), and clicks.
	void click(nana::window wd, nana::mouse button = nana::mouse::left_button);
	/// Presses and releases a key: a code of nana::keyboard (e.g. keyboard::os_arrow_down, keyboard::enter)
	/// or a character without modifiers (e.g. L'a', L'1').
	void key(wchar_t k);
	/// Lets the GUI thread process the events sent so far.
	void settle(unsigned ms = 300);
}
