#include <nana/gui/wvl.hpp>

int main()
{
    using namespace nana;

    form fm;
    drawing dw(fm);
    dw.draw([](paint::graphics& graph)
    {
        graph.rectangle(rectangle{5, 5, 50, 50}, 0xFF0000, true);
        graph.line(5, 5, 55, 55, 0xFFFFFF);
    });

    dw.update();
    fm.show();
    exec();
}
