#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

#include <iostream>
#include "../../nana/include/nana/gui/detail/general_events.hpp"
#include "../../nana/include/nana/gui/basis.hpp"
#include "../../nana/include/nana/gui/wvl.hpp"

void clicked(const nana::arg_click & eventinfo)
{
     std::cout<<  "When the window  fm  is clicked, this function is called. \n";
}



int main()
{
    using namespace nana;
    form fm;
    fm.events().click(clicked);
    fm.show();
	exec(2, [&fm]()
		{
			std::cout << "3 times automatic click. \n";
			fm.events().click.emit(nana::arg_click{});
			fm.events().click.emit(nana::arg_click{});
			fm.events().click.emit(nana::arg_click{});

			nana::arg_mouse m;
			m.window_handle=fm;
			m.alt=m.ctrl=m.mid_button=m.shift=false;
			m.left_button=true;
			m.pos.x=m.pos.y=1;
			m.button=::nana::mouse::left_button;

			std::cout << "Now with then mouse. \n";
			//fm.events().mouse_down.emit(m);
			//fm.events().mouse_up.emit(m);

			// char c;
			// std::cin >> c;

			//fm.close();

	     }, 1, &fm);
}