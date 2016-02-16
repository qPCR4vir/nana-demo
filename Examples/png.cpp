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
#ifdef    NANA_ENABLE_PNG
    pic.load( paint::image("../Examples/drawing.png") );
#elifdef   NANA_ENABLE_JPGG     
   // pic.load( paint::image("../Examples/drawing.jpg") );
#else 
    pic.load( paint::image("../Examples/Save.bmp") );
#endif
    fm.show();
    exec();
}
