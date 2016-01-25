#include <nana/gui/wvl.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream> 


int main()
{
    using namespace nana;
    filebox fb(0, true);
    fb.add_filter( ("Text File"),  ("*.text;*.doc"));
    fb.add_filter( ("All Files"),  ("*.*"));
    if(fb())
    {
        std::string file = fb.file();
        std::cout << "Selected file:  " << file << std::endl; 
    }
    char a;
    std::cin >> a;
}