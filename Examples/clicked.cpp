#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

void clicked(const nana::arg_mouse & eventinfo)
{
     //When the window  fm  is clicked, this function will be "called".
}

int main()
{
    using namespace nana;
    form fm;
    fm.events().click(clicked);
    fm.show();
    exec();
}