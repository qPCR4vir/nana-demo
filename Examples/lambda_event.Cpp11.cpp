#include <nana/gui/wvl.hpp> 
#include <iostream> 
int main() 
{ 
    nana::form form; 
    form.events().click( 
                         []{ std::cout<<"form is clicked"<<std::endl; } 
                        ); 
    form.show(); 
    nana::exec(); 
} 