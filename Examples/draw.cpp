#include <nana/gui/wvl.hpp>

int main()
{
    using namespace nana;

    form fm;
    drawing dw(fm);
    dw.draw([](paint::graphics& graph)
    {
        graph.rectangle(rectangle{5, 5, 50, 50}, true, colors::red );
        graph.line(point(5, 5), point(55, 55), colors::blue);
    });

    dw.update();
    fm.show();
    ::nana::exec();
}
