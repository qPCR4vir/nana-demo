#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/button.hpp> 
#include <iostream> 
void foo(const nana::gui::eventinfo& ei) 
{ 
    using namespace nana::gui; 
    form fm(ei.window, API::make_center(ei.window, 400, 300)); 
    fm.caption(STR("I am a modal form")); 
    std::cout<<"Block execution till modal form is closed"<<std::endl; 
    API::modal_window(fm); 
    std::cout<<"modal form is closed"<<std::endl; 
} 
int main() 
{ 
    using namespace nana::gui; 
    form fm; 
    fm.caption(STR("Click me to open a modal form")); 
    fm.make_event<events::click>(foo); 
    fm.show(); 
    exec(); 
} 