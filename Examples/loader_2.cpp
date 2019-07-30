#include <nana/gui.hpp> 
#include <nana/gui/widgets/button.hpp> 

void Click() 
{ 
    static int i;

    using namespace nana; 
    auto &fm=form_loader<form>()();
    fm.caption(std::to_string(++i)+ "-Nana window");
    fm.show(); 
} 


int main() 
{ 
    using namespace nana; 
    form fm{nana::rectangle(100, 100, 350, 230)}; 
    button btn(fm, nana::rectangle(10, 10, 150, 23)); 
    btn.caption("Open a new form"); 
    btn.events().click(Click); 
    fm.show(); 
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING
		1, 2, [&btn]() {click(btn); }
#endif

	);
} 
