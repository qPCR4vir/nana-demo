// The file dialog opened by a button and cancelled with Escape: show() returns no file.
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/filebox.hpp>
#include "test_input.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	using namespace nana;
	form fm;
	fm.caption("test_filebox");
	button open(fm, rectangle(10, 10, 100, 25));
	open.caption("Open...");

	std::atomic<int> state{ 0 };	// 1: the dialog is open, 2: show() returned
	std::atomic<int> files{ -1 };
	open.events().click([&] {
		filebox fb(fm, true);		// the dialog is modal: show() returns when it is closed
		state = 1;
		files = static_cast<int>(fb.show().size());
		state = 2;
	});
	fm.show();

	int failures = 0;
	auto wait_for = [&](int value) {
		for (int i = 0; i < 60 && state != value; ++i)
			std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
		return state == value;
	};

	exec(1, 1, [&] {
		namespace t = nana_test;
		t::click(open);
		bool const opened = wait_for(1);
		t::settle(800);					// let the dialog appear and take the focus
		t::key(keyboard::escape);
		bool const cancelled = wait_for(2) && (files == 0);
		std::cout << (opened ? "OK   " : "FAIL ") << "the dialog opens" << std::endl;
		std::cout << (cancelled ? "OK   " : "FAIL ") << "Escape cancels it: show() returns no file (" << files << ")" << std::endl;
		failures = !opened + !cancelled;
	});
	return failures;
}
