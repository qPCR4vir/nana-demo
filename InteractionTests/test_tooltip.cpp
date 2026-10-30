// Tooltips shown by hovering, and the form closed while a tooltip is visible.
// Closing the window of a visible tooltip once invalidated an iterator in nana's window manager (call_safe_place).
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/tooltip.hpp>
#include "test_input.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using namespace nana;
	form fm;
	fm.caption("test_tooltip");
	button first(fm, rectangle(10, 10, 100, 25)), second(fm, rectangle(10, 50, 100, 25)), close(fm, rectangle(10, 90, 100, 25));
	first.caption("First");
	second.caption("Second");
	close.caption("Close");
	tooltip::set(first, "the first tooltip");
	tooltip::set(second, "the second tooltip");
	tooltip::set(close, "closes the form");

	std::atomic<bool> closed{ false };
	close.events().click([&] { closed = true; fm.close(); });
	fm.show();

	int failures = 0;
	exec(1, 1, [&] {
		namespace t = nana_test;
		// hover each button long enough for its tooltip to appear (it shows after about 500 ms)
		for (auto* btn : { &first, &second, &close })
		{
			t::move(*btn, { 10, 12 });
			t::move(*btn);
			t::settle(1200);
		}
		// click Close while its tooltip is visible: the form and the tooltip are destroyed
		t::click();
		for (int i = 0; i < 40 && !closed; ++i)
			std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
		std::cout << (closed ? "OK   " : "FAIL ") << "form closed while a tooltip is visible" << std::endl;
		if (!closed) ++failures;
	});
	return failures;
}
