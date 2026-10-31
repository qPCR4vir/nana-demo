// The menu of a menubar, driven by real input: a click opens it, it stays open, and an item is chosen with the keyboard.
// It closed by itself right after opening in nana issue #579.
#include <nana/gui.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include "test_input.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using namespace nana;
	form fm;
	fm.caption("test_menubar");

	menubar mb(fm);
	mb.push_back("&File");
	std::atomic<bool> chosen{ false };
	mb.at(0).append("Item 1...", [&chosen](menu::item_proxy&) { chosen = true; });
	fm.show();

	int failures = 0;
	exec(1, 1, [&] {
		namespace t = nana_test;

		t::move(mb, { 15, 10 });		// "File" is the first item of the menubar
		t::click();
		t::settle(1000);				// the menu must still be open a second later
		t::key(keyboard::os_arrow_down);
		t::key(keyboard::enter);
		for (int i = 0; i < 40 && !chosen; ++i)
			std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
		std::cout << (chosen ? "OK   " : "FAIL ") << "menubar: File opened by a click, Item 1... chosen with the keyboard" << std::endl;
		if (!chosen) ++failures;
	});
	return failures;
}
