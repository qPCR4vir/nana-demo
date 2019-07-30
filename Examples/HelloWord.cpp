#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>

int main()
{
    using namespace nana;
    form    fm;
    label   lb(fm, rectangle(fm.size()));
    lb.caption("Hello, World");
    fm.show();
    exec();
}
