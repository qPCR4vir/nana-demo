#include <nana/gui/wvl.hpp>

void when_exit(const nana::gui::eventinfo& ei)
{
    nana::gui::msgbox m(ei.window, STR("msgbox example"), nana::gui::msgbox::yes_no);
    m.icon(m.icon_question);
    m<<STR("Are you sure you want to exit the game?");
    ei.unload.cancel = (m() != m.pick_yes);
}
int main()
{
    using namespace nana::gui;
    form fm;
    fm.make_event<events::unload>(when_exit);
    fm.show();
    exec();
}