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
    pic.load( "../Examples/a_png_file.png" );
    fm.show();
    exec();
}