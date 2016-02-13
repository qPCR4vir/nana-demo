#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/button.hpp> 

void click() 
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
    btn.events().click(click); 
    fm.show(); 
    exec(); 
} 
