#include "test_input.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

namespace nana_test
{
	namespace detail
	{
		void move_screen(int x, int y);
		void flush();
		bool input_goes_to_this_program();	// platform: false if the input would reach another program

		void ensure_target()
		{
			if (input_goes_to_this_program())
				return;
			std::cout << "SKIP the input would go to another program (on Windows: the test window could not take the"
				" foreground; run the test from a console in the foreground). No input was sent." << std::endl;
			std::_Exit(skip_code);
		}
	}

	void move(nana::window wd, nana::point pos)
	{
		nana::api::calc_screen_point(wd, pos);
		detail::ensure_target();
		detail::move_screen(pos.x, pos.y);
	}

	void move(nana::window wd)
	{
		auto const sz = nana::api::window_size(wd);
		move(wd, nana::point{ static_cast<int>(sz.width / 2), static_cast<int>(sz.height / 2) });
	}

	void click(nana::window wd, nana::mouse button)
	{
		// first onto wd, then to its center: if the pointer is already at the center, wd would see no move
		// (e.g. a button clicked again after a menu was closed would not react)
		auto const sz = nana::api::window_size(wd);
		move(wd, nana::point{ static_cast<int>(sz.width / 4), static_cast<int>(sz.height / 2) });
		settle(50);
		move(wd);
		settle(100);
		click(button);
	}

	void settle(unsigned ms)
	{
		detail::flush();
		std::this_thread::sleep_for(std::chrono::milliseconds{ ms });
	}
}
