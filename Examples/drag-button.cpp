#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/dragger.hpp>

int main()
{
    using namespace nana::gui;

    form fm;
    button btn(fm, nana::rectangle(10, 10, 100, 20));
    btn.caption(STR("Drag Me"));
    
    dragger dg;
    dg.trigger(btn);  //When you drag the btn, then
    dg.target(btn);   //move the btn

    fm.show();
    exec();
}