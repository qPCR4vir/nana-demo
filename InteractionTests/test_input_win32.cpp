// Windows: SendInput generates real input events (needs an interactive desktop session).
// DRAFT, not compiled nor tested on Linux: to be completed and tested on Windows.
#include "test_input.hpp"
#include <windows.h>

namespace nana_test
{
	namespace
	{
		WORD vkey(keys k)
		{
			switch (k)
			{
			case keys::down:	return VK_DOWN;
			case keys::up:		return VK_UP;
			case keys::left:	return VK_LEFT;
			case keys::right:	return VK_RIGHT;
			case keys::enter:	return VK_RETURN;
			case keys::escape:	return VK_ESCAPE;
			case keys::tab:		return VK_TAB;
			case keys::space:	return VK_SPACE;
			}
			return 0;
		}
	}

	namespace detail
	{
		void move_screen(int x, int y)
		{
			// absolute coordinates are normalized to 0..65535 over the virtual screen
			int const vx = ::GetSystemMetrics(SM_XVIRTUALSCREEN), vy = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
			int const vw = ::GetSystemMetrics(SM_CXVIRTUALSCREEN), vh = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
			INPUT in{};
			in.type = INPUT_MOUSE;
			in.mi.dx = static_cast<LONG>((x - vx) * 65535LL / (vw - 1));
			in.mi.dy = static_cast<LONG>((y - vy) * 65535LL / (vh - 1));
			in.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
			::SendInput(1, &in, sizeof(INPUT));
		}

		void flush() {}
	}

	void click(int button)
	{
		DWORD down = MOUSEEVENTF_LEFTDOWN, up = MOUSEEVENTF_LEFTUP;
		if (button == 2) { down = MOUSEEVENTF_MIDDLEDOWN; up = MOUSEEVENTF_MIDDLEUP; }
		else if (button == 3) { down = MOUSEEVENTF_RIGHTDOWN; up = MOUSEEVENTF_RIGHTUP; }
		INPUT in[2]{};
		in[0].type = in[1].type = INPUT_MOUSE;
		in[0].mi.dwFlags = down;
		in[1].mi.dwFlags = up;
		::SendInput(2, in, sizeof(INPUT));
	}

	void key(keys k)
	{
		INPUT in[2]{};
		in[0].type = in[1].type = INPUT_KEYBOARD;
		in[0].ki.wVk = in[1].ki.wVk = vkey(k);
		in[1].ki.dwFlags = KEYEVENTF_KEYUP;
		::SendInput(2, in, sizeof(INPUT));
	}
}
