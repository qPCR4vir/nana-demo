#include <nana/gui/wvl.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream> 


int main()
{
    using namespace nana;
    filebox fb(0, true);
    fb.add_filter(STR("Text File"), STR("*.text;*.doc"));
    fb.add_filter(STR("All Files"), STR("*.*"));
    if(fb())
    {
        nana::string file = fb.file();
        std::wcout << L"Selected file:  " << file << std::endl; 
    }
    char a;
    std::cin >> a;
}