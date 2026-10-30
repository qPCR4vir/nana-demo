// The popup menu, driven by real input: open it with a click, choose with the keyboard, then with the mouse.
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menu.hpp>
#include "test_input.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using namespace nana;
	form fm;
	fm.caption("test_menu");

	std::atomic<int> chosen{ -1 };
	menu mobj;
	auto on_item = [&chosen](menu::item_proxy& ip) { chosen = static_cast<int>(ip.index()); };
	mobj.append("Item 0", on_item);
	mobj.append_splitter();
	mobj.append("Item 1", on_item);

	button btn(fm, rectangle(10, 10, 100, 25));
	btn.caption("Popup Menu");
	btn.events().mouse_down(menu_popuper(mobj, btn, nana::point(100, 36), mouse::any_button));
	fm.show();

	int failures = 0;
	auto expect = [&](const char* what, int index) {
		for (int i = 0; i < 40 && chosen != index; ++i)
			std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
		bool const ok = (chosen == index);
		std::cout << (ok ? "OK   " : "FAIL ") << what << ": index " << chosen << " (expected " << index << ")" << std::endl;
		if (!ok) ++failures;
		chosen = -1;
	};

	exec(1, 1, [&] {
		namespace t = nana_test;

		// keyboard: Down selects "Item 0", Down skips the splitter to "Item 1" (index 2), Enter activates it
		t::click(btn);
		t::settle();
		t::key(keyboard::os_arrow_down);
		t::key(keyboard::os_arrow_down);
		t::key(keyboard::enter);
		expect("keyboard, Item 1", 2);

		// mouse: the menu pops up at (100, 36) of the button, with a 2-pixel border; hover "Item 0" and click it
		t::click(btn);
		t::settle();
		int const y = 36 + 2 + static_cast<int>(mobj.item_pixels()) / 2;
		t::move(btn, { 100 + 40, y });	// a move inside the item before the final position: the menu needs hover
		t::move(btn, { 100 + 50, y });
		t::settle();
		t::click();
		expect("mouse, Item 0", 0);
	});
	return failures;
}
