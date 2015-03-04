#include <nana/gui/wvl.hpp> 
#include <iostream> 
int main() 
{ 
    nana::gui::form form; 
    form.make_event<nana::gui::events::click>( 
        []{ std::cout<<"form is clicked"<<std::endl; } 
        ); 
    form.show(); 
    nana::gui::exec(); 
} 