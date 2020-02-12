#include <iostream>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>

void clicked(const nana::arg_click & eventinfo)
{
     std::cout<<  "When the window  fm  is clicked, this function is called. \n";
}

int main()
{
    nana::form fm;
    fm.caption("Click this nana windows");
    fm.events().click(clicked);
    fm.show();
    nana::exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

       1, 1, [&fm]() {  std::cout << "3 times automatic click. \n";
                        nana::click(fm); nana::click(fm); nana::click(fm);
                     }

#endif

	);
}