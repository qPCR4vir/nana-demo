#include <nana/gui.hpp>                  // always include this
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/msgbox.hpp>

int main()
 {
     using namespace nana;

     form   fm ;                             // Our main window
     fm.caption("My first NANA demo");       // (with this title)
     place  fm_place{fm};                    // have automatic layout
     bool   really_quick{false};             // and a default behavior
     label  hello{fm,"Hello World"};         // We put a label on our window
     button btn  {fm,"Quit"};                // and a button
     btn.tooltip( "I will ask first");       // that show a tip upon mouse hover
     group  act  {fm, "Actions"};            // Add a group of "options"

     act.add_option("Quick quickly")         // and two options that control quick behavior
        .events().click([&]()   {   really_quick=true;
                                    btn.tooltip("Quick quickly");       });
     act.add_option("Ask first")
        .events().click([&]()   {   really_quick=false;
                                    btn.tooltip("I will ask first");    });
     btn.events().click([&]()               // now the button know how to respond
                  {
                    if (!really_quick)     // not really quick !
                    {
                        msgbox m(fm,"Our demo", msgbox::yes_no) ;
                        m.icon(m.icon_question);
                        m << "Are you sure you want to quick?";
                        auto response= m();
                        if (response != m.pick_yes) return;   // return to the demo
                    }
                    API::exit();           // or really quick
                  });

     act.radio_mode(true);                   // Set "radio mode" (only one option selected)

       // let divide fm into fields to holds the other controls.
       // for example, let split fm into two fields separated by a movable vertical barre.
     fm_place.div("vertical <label margin=10>|70% <actions>");
     fm_place["label"  ]<< hello << btn;        // and place the controls there
     fm_place["actions"]<< act ;
     fm_place.collocate();                      // and collocate all in place

     fm.show();
     exec();
 }