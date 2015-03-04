#include <nana/gui/wvl.hpp>
#include <nana/gui/filebox.hpp>

int main()
{
    using namespace nana::gui;
    filebox fb(0, true);
    fb.add_filter(STR("Text File"), STR("*.text;*.doc"));
    fb.add_filter(STR("All Files"), STR("*.*"));
    if(fb())
    {
        nana::string file = fb.file();
    }
}