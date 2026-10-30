# Interaction tests

Tests that use nana widgets the way a user does: they move the mouse, hover, click and press keys, and then check what the
program did.

## Why

The other nana-demo programs are also run as tests (`NANA_CMAKE_AUTOMATIC_GUI_TESTING`), but almost all of them only open
their windows and close them again. That finds crashes at start-up and shutdown, not bugs that need a user: a menu item
that cannot be chosen, a tooltip that crashes when its window is closed, a dialog that does not open.

These tests send **real input events through the operating system**:

- X11 (Linux and other POSIX systems): the [XTest](https://www.x.org/releases/current/doc/libXtst/xtestlib.html)
  extension of the X server;
- Windows: [`SendInput`](https://learn.microsoft.com/windows/win32/api/winuser/nf-winuser-sendinput).

So the events go through nana's event loop exactly as a user's would: native event, coordinates, hit-testing, the GUI
thread and its locks. Emitting nana events directly (`api::emit_event`) would skip that part, where several real bugs were.

## Requirements

| Platform | To build | To run |
|---|---|---|
| Linux / X11 | the XTest development files (e.g. Debian/Ubuntu: `sudo apt install libxtst-dev`) | an X server with the XTest extension. A virtual one is enough, e.g. `Xvfb :99 -screen 0 1280x900x24 &` and then `DISPLAY=:99`. Not WSLg: there XTest does not move the pointer |
| Windows | nothing more (`SendInput` is in `user32`, already used by nana) | an interactive desktop session; do not touch the mouse or keyboard while the tests run |

If XTest is not found, CMake prints a warning and these tests are not built; everything else is unchanged.

## Build and run

These tests are built only with the test mode of nana-demo:

```sh
cmake -S nana-demo -B build -DNANA_CMAKE_AUTOMATIC_GUI_TESTING=ON
cmake --build build
cd build
ctest -R test_ --output-on-failure        # only the interaction tests (their names start with test_)
```

On Linux without a display, start Xvfb first (see above) and run `DISPLAY=:99 ctest ...`.

Each test prints one line per check (`OK` or `FAIL`) and returns the number of failures, so CTest reports it as failed if
any check fails.

## How a test works

A test is an ordinary nana program that uses the test mode of `nana::exec`:

```cpp
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include "test_input.hpp"

int main()
{
	using namespace nana;
	form fm;
	button btn(fm, rectangle(10, 10, 100, 25));
	bool clicked = false;
	btn.events().click([&] { clicked = true; });
	fm.show();

	int failures = 0;
	exec(1, 1, [&] {                     // 1 s to build the GUI, then this function runs in another thread
		nana_test::click(btn);           // real mouse move + click on the center of the button
		nana_test::settle();             // let the GUI thread process the events
		if (!clicked) ++failures;
	});                                  // then the test mode closes every window
	return failures;
}
```

`nana::exec(wait, wait_end, f)` exists when nana is built with `NANA_CMAKE_AUTOMATIC_GUI_TESTING`: it waits `wait`
seconds, runs `f` in a separate thread while the GUI thread processes the events, waits `wait_end` seconds and closes all
windows.

### The input helper (`test_input.hpp`, namespace `nana_test`)

| Function | What it does |
|---|---|
| `move(wd, pos)` | moves the pointer to `pos`, in the coordinates of window or widget `wd` |
| `move(wd)` | moves the pointer to the center of `wd` |
| `click(button = 1)` | presses and releases a mouse button where the pointer is (1 left, 2 middle, 3 right) |
| `click(wd, button = 1)` | moves to the center of `wd` and clicks |
| `key(keys::...)` | presses and releases a key: `down`, `up`, `left`, `right`, `enter`, `escape`, `tab`, `space` |
| `settle(ms = 300)` | waits so that the GUI thread can process the events sent so far |

Call these functions from the test function `f`, never from the GUI thread (an event handler): the GUI thread must stay
free to process the events.

### Tips

- **Check the state of the program, not pixels**: a callback that records what happened (an index, a flag), widget text,
  window existence. Wait for it with a short polling loop and a timeout, as `test_menu.cpp` does.
- **Some widgets need hover**: e.g. a menu item is chosen only if the pointer moved over it first. Move twice (near, then
  on the target) before clicking.
- **Popup windows** (menus, tooltips) are separate native windows: compute their position from the widget that opens
  them, or use the keyboard when the widget supports it.

## Tests

| Test | What it checks |
|---|---|
| `test_button` | the example above: a real click on a button runs its click handler |
| `test_menu` | a popup menu opened by clicking a button: an item chosen with the keyboard (Down, Down, Enter, skipping the splitter) and an item chosen with the mouse (hover and click) |

## Files

- `test_input.hpp`: the interface of the helper.
- `test_input_common.cpp`: the parts shared by all platforms (widget to screen coordinates with `api::calc_screen_point`).
- `test_input_x11.cpp`: X11 implementation (XTest, on its own connection to the X server).
- `test_input_win32.cpp`: Windows implementation (`SendInput`).
- `CMakeLists.txt`: builds the static library `nana_test_input` and one executable per test.

## Status

- X11: tested with Xvfb (Ubuntu 26.04, GCC 15 and clang 21).
- Windows: the `SendInput` implementation is a first version, not yet tested.
