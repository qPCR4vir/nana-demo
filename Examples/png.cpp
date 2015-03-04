#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/layout.hpp>
int main()
{
    using namespace nana::gui;
    form fm;
    picture pic(fm);
    gird gd(fm);
    gd.push(pic, 0, 0);
    pic.load(STR("a_png_file.png"));
    fm.show();
    exec();
}