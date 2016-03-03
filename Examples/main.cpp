#include <nana/gui/wvl.hpp>
#include <iostream>
int main()
{
    using namespace nana;
    std::cout << "Hello main... and nana/gui/wvl.hpp ";    
    form fm;
    std::cout << "After fm ";    
    //fm.events().click(clicked);
    fm.show();
    std::cout << "After fm.show() ";    
}
