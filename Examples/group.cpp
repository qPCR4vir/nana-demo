#include <nana/gui.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/label.hpp>
#include <iostream>

using namespace nana;

int main()
{

    form fm{API::make_center(600,400)};
    //fm.bgcolor(colors::mint_cream );
    place plc(fm);
    std::vector<std::unique_ptr<button>> btns;

    // the most external widgets
    label  out   {fm,  "This label is out of any group"};
    group  ext_gr{fm,  "An external <bold=true, color=blue>Group:</>", true };

    plc.div("vert gap=10 margin=5 <lab weight=30><all> ");
    plc["lab"]  << out ;
    plc["all"]  << ext_gr;

    // the external group contain:
    group grp_left {ext_gr,  ("A new <bold=true, color=0xff0000, font=\"Consolas\">Group:</>"), true };
    group grp_right{ext_gr,  ("A right <bold=true, color=0xff0000, font=\"Consolas\">Group:</>"), true};

    ext_gr.div("horizontal gap=3 margin=20  < <left_field> | 70% <right_field>> ");
    ext_gr["left_field"]  << grp_left;
    ext_gr["right_field"] << grp_right;

    // the left group
    grp_left.div("buttons vert gap=5 margin=3");


    // the right group
    group nested(grp_right.handle());
    label  lab {grp_right,  "A simple label "};
    button b1  {grp_right,  "add button"};
	b1.events().click([&grp_left, &btns]
	{
		btns.emplace_back(new button( grp_left,  "Button" ));

		grp_left["buttons"] << *btns.back();
		grp_left.collocate();
		std::cout << "button added\n";
	});
    button b2  {grp_right,  "button2"};
    button b3  {grp_right,  "button3"};
    grp_right.div( "<vertical margin=2 gap= 2 <vert lab> | 40% < <left_field> | 70% <right_field>> >");
    grp_right["lab"]  << lab.text_align(align::right) << nested ;
    grp_right["left_field"] << b1  ;
    grp_right["right_field"]<< b2 << b3;

    // the nested (rigth up) group
    label lab1{nested,  "A very simple group:"};
    button b4{nested,  "button4"};
    button b5{nested,  "button5"};
    button b6{nested,  "button6"};
    nested.div( " margin=3 min=30 gap= 2 all");
    nested["all"]  << lab1 << b4 << b5 << b6;


    plc.collocate();
    //grp1.plc.collocate();    // OK


	fm.show();

	::nana::exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&b1, &out, &grp_right]()
	{
		click(b1);
		click(b1);
		click(b1);
		click(out);
		click(grp_right);

	}
#endif

	);
}
