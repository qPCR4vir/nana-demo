#include <exception>
#include <iostream>

#include <nana/gui.hpp>
#include <nana/gui/animation.hpp>

int main()
{
    using namespace nana;
   
    //Build frames
    frameset fset;

    // please put the images in the "../Examples/" folder
    auto grs = {"000", "015", "030", "045", "060", "075", "090", "105", "120", "135", "150", "170",
                "180", "195", "210", "225", "240", "260", "270", "285", "300", "315", "330", "345"};
    
    try
    {   
        for (const auto& gr: grs)
        {
            auto filename = std::string("../Examples/a_pic") + gr + ".bmp";
            nana::paint::image img(filename);

            if (img.empty()) throw std::runtime_error("Failed to load image: " + filename);

            fset.push_back(nana::paint::image(std::string("../Examples/a_pic") + gr + ".bmp"));
        }
        //A widget to display animation.
        form fm;
        fm.show();

        animation ani;
        ani.push_back(fset);
        ani.output(fm, nana::point());
        ani.looped(true);
        ani.play();

        exec();

    } catch ( std::exception& e )
    {
        std::cerr << e.what();
        // std::getchar();
        return -1;
    }

    return 0;
}
