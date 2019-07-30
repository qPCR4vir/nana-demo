#include <nana/gui.hpp> 
#include <nana/gui/widgets/button.hpp> 
#include <iostream> 
void foo(const nana::arg_click& ei)
{ 
    using namespace nana; 
    form fm(ei.window_handle, API::make_center(ei.window_handle, 400, 300)); 
    fm.caption(("I am a modal form")); 
    std::cout<<"Block execution till modal form is closed"<<std::endl; 
    API::modal_window(fm); 
    std::cout<<"modal form is closed"<<std::endl; 
} 
int main() 
{ 
    using namespace nana; 
    form fm; 
    fm.caption(("Click me to open a modal form")); 
    fm.events().click(foo); 
    fm.show(); 
    exec(); 
} 