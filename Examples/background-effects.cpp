
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>

int main()
{
    using namespace nana::gui;

    form fm;

    button btn(fm, L"Normal Button");

    button btn0(fm, L"Basic Mode");

    //It is basic mode when the third parameter of effects_bground() is specified by zero.
    API::effects_bground(btn0, effects::bground_transparent(0), 0);

    button btn1(fm, L"Blend Mode: Transparent");
    API::effects_bground(btn1, effects::bground_transparent(0), 0.5);

    button btn2(fm, L"Blend Mode: Blur");
    API::effects_bground(btn2, effects::bground_blur(2), 0.5);

    label lb0(fm, L"Normal Label");
    lb0.foreground(0xFFFFFF);

    label lb1(fm, L"Basic Mode: Transparent");
    lb1.foreground(0xFFFFFF);
    //Equal to lb1.transparent(true);
    API::effects_bground(lb1, effects::bground_transparent(0), 0);

    label lb2(fm, L"Basic Mode: Transparent and blend with its background color");
    lb2.foreground(0xFFFFFF);
    lb2.background(0xFF0000);
    API::effects_bground(lb2, effects::bground_transparent(10), 0);

    label lb3(fm, L"Basic Mode: Blur");
    lb3.foreground(0xFFFFFF);
    API::effects_bground(lb3, effects::bground_blur(2), 0);

    place pl(fm);
    pl.div("<><weight=80% abc vertical><>");
    pl.field("abc")<<5<<btn<<5<<btn0<<5<<btn1<<5<<btn2<<5<<lb0<<5<<lb1<<5<<lb2<<5<<lb3<<5;
    pl.collocate();

    fm.show();

    nana::paint::image img(L"image.bmp");
    drawing dw(fm);
    dw.draw([&img](nana::paint::graphics & graph)
    {
        img.paste(graph, 0, 0);
    });
    dw.update();

    exec();
}