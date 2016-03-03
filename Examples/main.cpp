#include <nana/gui/wvl.hpp>
#include <iostream>
#include <exception>

int main()
{
    using namespace nana;
    std::cout << "Hello main... and nana/gui/wvl.hpp " << std::endl;    
    try {
    form fm;
    }
    catch (std::exception& e)
    {
            std::cout << e.what() << std::endl; 
    }
    catch (...) 
    {
        cout << "Exception occurred" << std::endl; 
    }
    std::cout << "After fm " << std::endl; 
    //fm.events().click(clicked);
    //fm.show();
    //std::cout << "After fm.show() " << std::endl;  
}
