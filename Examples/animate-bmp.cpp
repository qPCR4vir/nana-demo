#include <nana/gui.hpp>
#include <nana/gui/animation.hpp>

int main()
{
    using namespace nana;
   
    //Build frames
    frameset fset;
    auto grs = {"000", "015", "030", "045", "060", "075", "090", "105", "120", "135", "150", "170",
                "180", "195", "210", "225", "240", "260", "270", "285", "300", "315", "330", "345"};
    for (const auto& gr: grs)
        fset.push_back(nana::paint::image(std::string("../Examples/a_pic")+gr+".bmp"));

    //A widget to display animation.
    form fm;
    fm.show();

    animation ani;
    ani.push_back(fset);
    ani.output(fm, nana::point());
    ani.looped(true);
    ani.play();

    exec();
}
