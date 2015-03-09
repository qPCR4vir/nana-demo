#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>

int main()
{
    using namespace nana;
    form    fm;
    label   lb(fm, fm.size());
    lb.caption(STR("Hello, World"));
    fm.show();
    exec();
}