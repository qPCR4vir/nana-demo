#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/place.hpp>

int main()
{
    using namespace nana;
    form fm;
    picture pic(fm);
	place p{fm};
    p.div("pic"); 
	p["pic"] << pic ;
#ifdef ENABLE_PNG
    pic.load( "../Examples/drawing.png" );
#endif
    fm.show();
    exec();
}