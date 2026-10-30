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

		KeySym keysym(keys k)
		{
			switch (k)
			{
			case keys::down:	return XK_Down;
			case keys::up:		return XK_Up;
			case keys::left:	return XK_Left;
			case keys::right:	return XK_Right;
			case keys::enter:	return XK_Return;
			case keys::escape:	return XK_Escape;
			case keys::tab:		return XK_Tab;
			case keys::space:	return XK_space;
			}
			return NoSymbol;
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

	void click(int button)
	{
		::XTestFakeButtonEvent(display(), static_cast<unsigned>(button), True, CurrentTime);
		::XTestFakeButtonEvent(display(), static_cast<unsigned>(button), False, CurrentTime);
		::XFlush(display());
	}

	void key(keys k)
	{
		auto const code = ::XKeysymToKeycode(display(), keysym(k));
		::XTestFakeKeyEvent(display(), code, True, CurrentTime);
		::XTestFakeKeyEvent(display(), code, False, CurrentTime);
		::XFlush(display());
	}
}
