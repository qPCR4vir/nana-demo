
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <iostream>

int main()
{
    using namespace nana;

    form fm;

    button btn(fm, "Normal Button");
	btn.events().click([]() {std::cout << "Normal Button clicked.\n"; });

    button btn0(fm, "Basic Mode");

    //It is basic mode when the third parameter of effects_bground() is specified by zero.
    API::effects_bground(btn0, effects::bground_transparent(0), 0);

    button btn1(fm, "Blend Mode: Transparent");
    API::effects_bground(btn1, effects::bground_transparent(0), 0.5);

    button btn2(fm, "Blend Mode: Blur");
    API::effects_bground(btn2, effects::bground_blur(2), 0.5);

    label lb0(fm,  ("Normal Label"));
    //lb0.fgcolor(color_rgb(0xFFFFFF));
	lb0.events().click([]() {std::cout << "Label clicked too.\n"; });

    label lb1(fm,  ("Basic Mode: Transparent"));
    lb1.fgcolor(color_rgb(0xFFFFFF));
    //Equal to lb1.transparent(true);
    API::effects_bground(lb1, effects::bground_transparent(0), 0);

    label lb2(fm,  ("Basic Mode: Transparent and blend with its background color"));
    lb2.fgcolor(color_rgb(0xFFFFFF));
    lb2.bgcolor(color_rgb(0xFF0000));
    API::effects_bground(lb2, effects::bground_transparent(10), 0);

    label lb3(fm, "Basic Mode: Blur");
    lb3.fgcolor(color_rgb(0xFFFFFF));
    API::effects_bground(lb3, effects::bground_blur(2), 0);

    place pl(fm);
    pl.div("<><weight=80% abc vertical gap=5><>");
    pl["abc"] <<  btn << btn0  << btn1 << btn2<< lb0<< lb1<< lb2<< lb3 ;
    pl.collocate();

    fm.show();

    nana::paint::image img("..\\Examples\\image02.bmp");
    drawing dw(fm);
    dw.draw([&img](nana::paint::graphics & graph)
    {
		if (img.empty()) return;
		img.paste(graph, nana::point{} );
    });
    dw.update();

	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING
		1, 1, [&btn,&lb0]() 
	      {
		     click(btn); 
			 click(lb0); 
	      }
#endif
	
	    );
}
