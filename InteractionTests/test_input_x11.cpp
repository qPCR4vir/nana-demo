// X11: XTest generates real input events in the X server (also in Xvfb; not in WSLg, where the pointer does not move).
#include "test_input.hpp"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <xcb/xcb.h>
#include <cstdlib>
#include <iostream>
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

		void ensure_target();
		extern nana::native_window_type target_root;

		// XTest events go to the X server, which gives them to the window under the pointer (buttons) or with the
		// keyboard focus (keys), as with a user. A move cannot activate anything: click() and key() check the target.
		bool input_goes_to_this_program()
		{
			return true;
		}
	}

	namespace
	{
		// The X server gives each connection a base for the ids of its resources: the windows created by this test (by
		// nana's connection) share the bits outside the resource id mask, and the windows of other programs do not.
		bool of_this_test(Window wd)
		{
			static unsigned long const mask = [] {
				unsigned long m = 0;
				if (auto conn = ::xcb_connect(nullptr, nullptr))
				{
					if (!::xcb_connection_has_error(conn))
						m = ::xcb_get_setup(conn)->resource_id_mask;
					::xcb_disconnect(conn);
				}
				return m;
			}();

			auto const own = reinterpret_cast<Window>(detail::target_root);
			return mask && own && wd && ((wd & ~mask) == (own & ~mask));
		}

		// the deepest window under the pointer
		Window under_pointer()
		{
			Window wd = DefaultRootWindow(display()), root, child;
			int root_x, root_y, x, y;
			unsigned mask;
			while (::XQueryPointer(display(), wd, &root, &child, &root_x, &root_y, &x, &y, &mask) && child != None)
				wd = child;
			return wd;
		}

		// input must never reach another program: send nothing more and end the test as skipped
		void skip_unless(bool ours, const char* what)
		{
			if (ours)
				return;
			std::cout << "SKIP " << what << " is not a window of this test (another window covers it?). No input was sent."
				<< std::endl;
			std::_Exit(skip_code);
		}
	}

	void click(nana::mouse button)
	{
		detail::ensure_target();
		skip_unless(of_this_test(under_pointer()), "the window under the pointer");
		unsigned const xbutton = (button == nana::mouse::middle_button ? 2 : (button == nana::mouse::right_button ? 3 : 1));
		::XTestFakeButtonEvent(display(), xbutton, True, CurrentTime);
		::XTestFakeButtonEvent(display(), xbutton, False, CurrentTime);
		::XFlush(display());
	}

	void key(wchar_t k)
	{
		detail::ensure_target();
		Window focus;
		int revert_to;
		::XGetInputFocus(display(), &focus, &revert_to);
		// PointerRoot: the keys go to the window under the pointer
		skip_unless(of_this_test(focus == PointerRoot ? under_pointer() : focus), "the window with the keyboard focus");
		auto const code = ::XKeysymToKeycode(display(), keysym(k));
		::XTestFakeKeyEvent(display(), code, True, CurrentTime);
		::XTestFakeKeyEvent(display(), code, False, CurrentTime);
		::XFlush(display());
	}
}
