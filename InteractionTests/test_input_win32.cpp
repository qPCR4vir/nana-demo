// Windows: SendInput generates real input events (needs an interactive desktop session).
// DRAFT, not compiled nor tested on Linux: to be completed and tested on Windows.
#include "test_input.hpp"
#include <windows.h>

namespace nana_test
{
	namespace
	{
		// the system codes of nana::keyboard are virtual-key codes; 0 for a character
		WORD vkey(wchar_t k)
		{
			using nana::keyboard;
			switch (k)
			{
			case keyboard::os_arrow_down: case keyboard::os_arrow_up: case keyboard::os_arrow_left: case keyboard::os_arrow_right:
			case keyboard::os_pageup: case keyboard::os_pagedown: case keyboard::os_home: case keyboard::os_end:
			case keyboard::os_insert: case keyboard::os_shift: case keyboard::os_ctrl: case keyboard::alt:
			case keyboard::enter: case keyboard::escape: case keyboard::tab: case keyboard::backspace: case keyboard::space:
				return static_cast<WORD>(k);
			case keyboard::enter_n:	return VK_RETURN;
			case keyboard::del:		return VK_DELETE;
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

	void click(nana::mouse button)
	{
		DWORD down = MOUSEEVENTF_LEFTDOWN, up = MOUSEEVENTF_LEFTUP;
		if (button == nana::mouse::middle_button) { down = MOUSEEVENTF_MIDDLEDOWN; up = MOUSEEVENTF_MIDDLEUP; }
		else if (button == nana::mouse::right_button) { down = MOUSEEVENTF_RIGHTDOWN; up = MOUSEEVENTF_RIGHTUP; }
		INPUT in[2]{};
		in[0].type = in[1].type = INPUT_MOUSE;
		in[0].mi.dwFlags = down;
		in[1].mi.dwFlags = up;
		::SendInput(2, in, sizeof(INPUT));
	}

	void key(wchar_t k)
	{
		INPUT in[2]{};
		in[0].type = in[1].type = INPUT_KEYBOARD;
		if (auto const vk = vkey(k))
			in[0].ki.wVk = in[1].ki.wVk = vk;
		else
		{
			// a character: sent as Unicode, independent of the keyboard layout
			in[0].ki.wScan = in[1].ki.wScan = static_cast<WORD>(k);
			in[0].ki.dwFlags = KEYEVENTF_UNICODE;
		}
		in[1].ki.dwFlags = in[0].ki.dwFlags | KEYEVENTF_KEYUP;
		::SendInput(2, in, sizeof(INPUT));
	}
}
