#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/menubar.hpp>

int main(int argc, char **argv)
{
    nana::form fm;
    nana::place pl(fm);
    nana::menubar mn(fm);

    pl.div("<vertical <menu weight=25>>");
    pl.field("menu") << mn; 

    mn.push_back("test");
    mn.at(0).append("open modal window", [&fm](nana::menu::item_proxy& ip)
    {
        nana::form fm2(fm);
        fm2.show();
        fm2.modality();
    });

    pl.collocate();

    fm.show();

    nana::exec();

    return 0;
}

 // NOW FIXED !!
    //After opening fm2 via the Menu and closing the Form, the error occurs.

    //last callstack entry is:
    //0 0x00000000005781b6 std::unique_ptr<nana::basic_event<nana::arg_mouse>::docker, std::default_delete<nana::basic_event<nana::arg_mouse>::docker> >::~unique_ptr

    //if i understand correctly, the error occurs in the destructor of an unique_ptr.

    //The system i'm using is nana with the latest master branch, windows 8.1 with mingw (gcc 4.9.2).

    //If you need further information please let me know.

    //best regards, jan
    //^

    //additional note:
    //if you open a window via a button or something else, no error occurs. this makes me guess the error is relating to menubars
