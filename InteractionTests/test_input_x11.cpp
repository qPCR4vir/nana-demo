// X11: XTest generates real input events in the X server (also in Xvfb; not in WSLg, where the pointer does not move).
#include "test_input.hpp"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <stdexcept>

namespace nana_test
{
	namespace
	{
		// own connection: nana's connection belongs to the GUI thread
		Display* display()
		{
			static Display* const disp = [] {
				auto d = ::XOpenDisplay(nullptr);
				int ev, err, major, minor;
				if (!d || !::XTestQueryExtension(d, &ev, &err, &major, &minor))
					throw std::runtime_error("nana_test: no X display with the XTest extension");
				return d;
			}();
			return disp;
		}

		KeySym keysym(wchar_t k)
		{
			using nana::keyboard;
			switch (k)
			{
			case keyboard::os_arrow_down:	return XK_Down;
			case keyboard::os_arrow_up:		return XK_Up;
			case keyboard::os_arrow_left:	return XK_Left;
			case keyboard::os_arrow_right:	return XK_Right;
			case keyboard::os_pageup:		return XK_Prior;
			case keyboard::os_pagedown:		return XK_Next;
			case keyboard::os_home:			return XK_Home;
			case keyboard::os_end:			return XK_End;
			case keyboard::os_insert:		return XK_Insert;
			case keyboard::os_shift:		return XK_Shift_L;
			case keyboard::os_ctrl:			return XK_Control_L;
			case keyboard::alt:				return XK_Alt_L;
			case keyboard::enter:
			case keyboard::enter_n:			return XK_Return;
			case keyboard::escape:			return XK_Escape;
			case keyboard::tab:				return XK_Tab;
			case keyboard::backspace:		return XK_BackSpace;
			case keyboard::del:				return XK_Delete;
			}
			// characters: the keysyms of Latin-1 are their codes; the others are 0x01000000 + Unicode
			return (k < 0x100 ? static_cast<KeySym>(k) : static_cast<KeySym>(0x01000000 | k));
		}
	}

	namespace detail
	{
		void move_screen(int x, int y)
		{
			::XTestFakeMotionEvent(display(), -1, x, y, CurrentTime);
			::XFlush(display());
		}

		void flush()
		{
			::XSync(display(), False);
		}
	}

	void click(nana::mouse button)
	{
		unsigned const xbutton = (button == nana::mouse::middle_button ? 2 : (button == nana::mouse::right_button ? 3 : 1));
		::XTestFakeButtonEvent(display(), xbutton, True, CurrentTime);
		::XTestFakeButtonEvent(display(), xbutton, False, CurrentTime);
		::XFlush(display());
	}

	void key(wchar_t k)
	{
		auto const code = ::XKeysymToKeycode(display(), keysym(k));
		::XTestFakeKeyEvent(display(), code, True, CurrentTime);
		::XTestFakeKeyEvent(display(), code, False, CurrentTime);
		::XFlush(display());
	}
}
