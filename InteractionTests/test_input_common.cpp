#include "test_input.hpp"
#include <chrono>
#include <thread>

namespace nana_test
{
	namespace detail { void move_screen(int x, int y); void flush(); }

	void move(nana::window wd, nana::point pos)
	{
		nana::api::calc_screen_point(wd, pos);
		detail::move_screen(pos.x, pos.y);
	}

	void move(nana::window wd)
	{
		auto const sz = nana::api::window_size(wd);
		move(wd, nana::point{ static_cast<int>(sz.width / 2), static_cast<int>(sz.height / 2) });
	}

	void click(nana::window wd, int button)
	{
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
