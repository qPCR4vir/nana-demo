#include <nana/gui/wvl.hpp>

int main()
{
    using namespace nana::gui;

    form fm;
    drawing dw(fm);
    dw.draw([](nana::paint::graphics& graph)
    {
        graph.rectangle(nana::rectangle(5, 5, 50, 50), 0xFF0000, true);
        graph.line(5, 5, 55, 55, 0xFFFFFF);
    });

    dw.update();
    fm.show();
    exec();
}

//C++03
//
//#include <nana/gui/wvl.hpp>
//
//void draw_method(nana::paint::graphics& graph)
//{
//    graph.rectangle(nana::rectangle(5, 5, 10, 10), 0xFF0000, true);
//}
//
//int main()
//{
//    using namespace nana::gui;
//
//    form fm;
//    drawing dw(fm);
//    dw.draw(draw_method);
//
//    dw.update();
//    fm.show();
//    exec();
//}


/// \todo docs: separate examples Cpp03 and 11 in differents directories: examples-Cpp03 and *11 for the exaples wich difier