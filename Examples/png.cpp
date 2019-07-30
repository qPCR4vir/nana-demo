#include <nana/gui.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/place.hpp>
int main()
{
    using namespace nana;
    form fm;
    picture picPNG  {fm},
            picJPEG {fm},
            picBMP  {fm};

    place p{fm};
    p.div("pic"); 

#ifdef  NANA_ENABLE_PNG
    picPNG.load( paint::image("../Examples/png.png") );
    p["pic"] << picPNG ;
#endif

#ifdef  NANA_ENABLE_JPEG
    picJPEG.load( paint::image("../Examples/jpeg.jpg") );
    p["pic"] << picJPEG ;
#endif

    picBMP.load( paint::image("../Examples/bmp.bmp") );
    p["pic"] << picBMP ;
    p.collocate();

    fm.show();
    exec();
}
