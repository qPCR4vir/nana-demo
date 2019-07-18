#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/dragger.hpp>

int main()
{
    using namespace nana;

    form fm;
    button btn(fm, nana::rectangle(10, 10, 100, 20));
    btn.caption("Drag Me");
    
    dragger dg;
    dg.trigger(btn);  //When you drag the btn, then
    dg.target(btn);   //move the btn

    fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&btn]() {click(btn); }
#endif

	);
}
