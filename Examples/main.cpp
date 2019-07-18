#include <nana/gui.hpp>
#include <iostream>
#include <exception>

int main()
{
    using namespace nana;
    std::cout << "Hello main... and nana/gui.hpp " << std::endl;    
    try {
            form fm;
            std::cout << "After fm " << std::endl; 
            //fm.events().click(clicked);
            fm.show();
            std::cout << "After fm.show() " << std::endl;  
            exec();
    }
    catch (std::exception& e)
    {
            std::cout << e.what() << std::endl; 
    }
    catch (...) 
    {
        std::cout << "Exception occurred" << std::endl; 
    }

}
