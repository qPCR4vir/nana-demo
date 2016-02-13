#include<nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

int main()
 {
     using namespace nana;

     form fm;
     fm.caption("Hello World");
     button btn(fm, nana::rectangle(20, 20, 150, 30));
     btn.caption("Quit");
     btn.events().click(API::exit);
     fm.show();
     exec();
 }