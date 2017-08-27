#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

#include <iostream>




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
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&fm]()
		{
			std::cout << "3 times automatic click. \n";
			click(fm);
			click(fm);
			click(fm);

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

	     }
#endif

	);
}