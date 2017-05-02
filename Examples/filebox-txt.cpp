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
// see:  https://github.com/cnjinhao/nana/issues/191#issuecomment-287226668
// and:  http://nanapro.org/en-us/documentation/utilities/filebox.htm
//#include <nana/gui.hpp>
//#include <nana/gui/filebox.hpp>
//
//int main()
//{
//    using namespace nana;
//    filebox fb(true);
//    fb.add_filter("Text File", "*.text;*.doc");
//    fb.add_filter("All Files", "*.*");
//
//    if (fb())	//When user clicked 'OK'
//    {
//        auto file = fb.file();
//    }
//}