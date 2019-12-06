#include<nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>

int main()
 {
     nana::form fm;
     fm.caption("Hello World");
     nana::button btn(fm, nana::rectangle(20, 20, 150, 30));
     btn.caption("Quit");
     btn.events().click(nana::API::exit_all);
     fm.show();
     nana::exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING
     2, 1, [&btn]() { click(btn); }
#endif

	 );
 }