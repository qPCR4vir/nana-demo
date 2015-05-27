#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/label.hpp>

// This example need nana/gui/widgets/group.hpp and the corresponding group.cpp. 
// It can be found in branch:
// https://github.com/cnjinhao/nana/tree/feature-group
// or in:
// https://github.com/qPCR4vir/nana
// for exapmle in: https://github.com/qPCR4vir/nana/blob/master/include/nana/gui/widgets/group.hpp
// and: https://github.com/qPCR4vir/nana/blob/master/source/gui/widgets/group.cpp

using namespace nana;

template<typename T, typename ...Args>
T* add_widget(Args&&... args)
{
	widgets_.emplace_back(new T(*this, std::forward<Args>(args)...));

	plc_["abc"] << widgets_.back()->handle();
	plc_.collocate();
	return static_cast<T*>(widgets_.back().get());
}

int main()
{

    form fm{API::make_center(600,400)};
    //fm.bgcolor(colors::mint_cream );
    place plc(fm);
    std::vector<std::unique_ptr<button>> btns;

    // the most external widgets
    label  out   {fm, STR("This label is out of any group")};
    group  ext_gr{fm, STR("An external <bold=true, color=blue>Group:</>"), true };

    plc.div("vert gap=10 margin=5 <lab weight=30><all> ");
    plc["lab"]  << out ;
    plc["all"]  << ext_gr;

    // the external group contain:
    group grp_left {ext_gr.inner(), STR("A new <bold=true, color=0xff0000, font=\"Consolas\">Group:</>"), true };
    group grp_right{ext_gr.inner(), STR("A right <bold=true, color=0xff0000, font=\"Consolas\">Group:</>"), true};

    ext_gr.plc().div("horizontal gap=3 margin=20  < <left> | 70% <right>> ");
    ext_gr.plc()["left"]  << grp_left;
    ext_gr.plc()["right"] << grp_right;

    // the left group
    grp_left.plc().div("buttons vert gap=5 margin=3");


    // the right group
    group nested(grp_right.inner() );
    label  lab {grp_right.inner(), STR("A simple label ")};
    button b1  {grp_right.inner(), STR("add button")};
	b1.events().click([&grp_left, &btns]
	{
		btns.emplace_back(new button( grp_left.inner(), STR("Button") ));

		grp_left.plc()["buttons"] << *btns.back();
		grp_left.plc().collocate();
	});
    button b2  {grp_right.inner(), STR("button2")};
    button b3  {grp_right.inner(), STR("button3")};
    grp_right.plc().div( "<vertical margin=2 gap= 2 <vert lab> | 40% < <left> | 70% <right>> >");
    grp_right.plc()["lab"]  << lab.text_align(align::right) << nested ;
    grp_right.plc()["left"] << b1  ;
    grp_right.plc()["right"]<< b2 << b3;

    // the nested (rigth up) group
    label lab1{nested.inner(), STR("A very simple group:")};
    button b4{nested.inner(), STR("button4")};
    button b5{nested.inner(), STR("button5")};
    button b6{nested.inner(), STR("button6")};
    nested.plc().div( " margin=3 min=30 gap= 2 all");
    nested.plc()["all"]  << lab1 << b4 << b5 << b6;


    plc.collocate();
    //grp1.plc.collocate();    // OK


	fm.show();

	::nana::exec();
}
