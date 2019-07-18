#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>

using namespace nana;

void listener(label::command cmd, const std::string& s)
{
    if(label::command::click == cmd)
    {
        msgbox mb(("target clicked"));
        mb<<("the target \"")<<s<<"\" is clicked";
        mb();
    }
}

int main()
{
    form fm;
    label lab(fm, nana::rectangle(0, 0, 100, 40));
    lab.format(true);
    lab.add_format_listener(listener);
    lab.caption(("Click <color=0xFF target=\"target id\">here</>"));
    fm.show();
    exec();
}
