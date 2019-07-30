    #include <nana/gui.hpp>
    #include <nana/gui/place.hpp>
    #include <nana/gui/widgets/button.hpp>
    #include <nana/gui/widgets/textbox.hpp>

    int main()
    {
        using namespace nana;

                 // Define widgets
        form fm;
        textbox usr  {fm},   
                pswd {fm};
        button  login {fm, "Login"}, 
                cancel{fm, "Cancel"};

        usr .tip_string("User:"    ).multi_lines(false);
        pswd.tip_string("Password:").multi_lines(false).mask('*');

                // Define a place for the form.
        place plc {fm};

                // Divide the form into fields
                
        //plc.div("margin= 10%   gap=20 vertical< weight=70 gap=20 vertical textboxs arrange=[25,25]> <min=20> <weight=25 gap=10 buttons>  > ");

        plc.div("<><weight=80% vertical<><weight=70% vertical <vertical gap=10 textboxs arrange=[25,25]>  <weight=25 gap=10 buttons> ><>><>");

        //Insert widgets

        //The field textboxs is vertical, it automatically adjusts the widgets' top and height. 
        plc.field("textboxs")<< usr  << pswd ;

        plc.field("buttons") <<login << cancel;

        // Finially, the widgets should be collocated.
        // Do not miss this line, otherwise the widgets are not collocated
        // until the form is resized.
        plc.collocate();

        fm.show();
        exec();
    }