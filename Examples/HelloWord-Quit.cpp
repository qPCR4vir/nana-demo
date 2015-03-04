#include<nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

int main()
 {
     using namespace nana::gui;

     form fm;
     fm.caption(STR("Hello World"));
     button btn(fm, nana::rectangle(20, 20, 150, 30));
     btn.caption(STR("Quit"));
     btn.make_event<events::click>(API::exit);
     fm.show();
     exec();
 }