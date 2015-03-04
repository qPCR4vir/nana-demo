#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/button.hpp> 
void click() 
{ 
    using namespace nana::gui; 
    form_loader<form>()().show(); 
} 
int main() 
{ 
    using namespace nana::gui; 
    form fm; 
    button btn(fm, nana::rectangle(10, 10, 150, 23)); 
    btn.caption(STR("Open a new form")); 
    btn.make_event<events::click>(click); 
    fm.show(); 
    exec(); 
} 
