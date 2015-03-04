    #include <nana/gui/wvl.hpp>
    #include <nana/gui/place.hpp>
    #include <nana/gui/widgets/button.hpp>
    #include <nana/gui/widgets/textbox.hpp>

    int main()
    {
        using namespace nana::gui;

                 //Define widgets
        form fm;
        textbox usr(fm), pswd(fm);
        button login(fm), cancel(fm);

        usr .tip_string(STR("User:")    ).multi_lines(false);
        pswd.tip_string(STR("Password:")).multi_lines(false).mask('*');

        login .caption(STR("Login"));
        cancel.caption(STR("Cancel"));

                //Define a place for the form.
        place plc(fm);

                //Divide the form into fields
        plc.div("<><weight=80% vertical<><weight=70% vertical<vertical textboxs><weight=25 buttons>><>><>");

        //Insert widgets

        //The field textboxs is vertical, it automatically adjusts the widgets' top
        //and height. The usr and pswd are single-line textboxs, and we should specify
        //them with a fixed height.
        plc.field("textboxs") << plc.fixed(usr, 25) << 10 << plc.fixed(pswd, 25);

        plc.field("buttons") << login << 10 << cancel;

        //Finially, the widgets should be collocated.
        //Do not miss this line, otherwise the widgets are not collocated
        //until the form is resized.
        plc.collocate();

        fm.show();
        exec();
    }