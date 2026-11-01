// The example of README.md: a real click on a button runs its click handler.
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include "test_input.hpp"
#include <atomic>
#include <iostream>

int main()
{
	using namespace nana;
	form fm;
	button btn(fm, rectangle(10, 10, 100, 25));
	btn.caption("Click me");
	std::atomic<bool> clicked{ false };	// written by the GUI thread, read by the thread of the test
	btn.events().click([&] { clicked = true; });
	fm.show();

	int failures = 0;
	exec(1, 1, [&] {
		nana_test::click(btn);
		nana_test::settle();
		std::cout << (clicked ? "OK   " : "FAIL ") << "click on the button" << std::endl;
		if (!clicked) ++failures;
	});
	return failures;
}
