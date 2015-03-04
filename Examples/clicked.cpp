#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

void clicked(const nana::gui::eventinfo&)
{
     //When the window  fm  is clicked, this function will be "called".
}

int main()
{
    using namespace nana::gui;
    form fm;
    fm.make_event<events::click>(clicked);
    fm.show();
    exec();
}