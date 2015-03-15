
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/drawing.hpp>

using namespace nana;
namespace dm
{
    class group
	    : public panel<true>
    {
    public:
	    template<typename T, typename ...Args>
	    T* create_widget(Args&&... args)
	    {
		    widgets_.emplace_back(new T(*this, std::forward<Args>(args)...));

		    plc_["abc"] << widgets_.back()->handle();
		    plc_.collocate();
		    return static_cast<T*>(widgets_.back().get());
	    }
    private:
	    void _m_complete_creation() override
	    {
		    //define an etiquete
		    plc_.bind(*this);
		    plc_.div("abc vert gap=5 margin=3");

		    //draw a simple border.
		    drawing dw(*this);
		    dw.draw([](paint::graphics& graph)
		    {
			    graph.rectangle(false, colors::gray_border);
		    });
	    }
    private:
	    place plc_;
	    std::vector<std::unique_ptr<widget>> widgets_;
    };

    class group1: public panel<true>
    {
    public:
        place           plc   {*this};
        label           titel/*  {*this}*/;
        std::string     fmt;

        group1(widget &owner, ::nana::string titel_={}, bool format=false, unsigned gap=2, rectangle r={})
            : panel  (owner, r), 
              titel  (*this, titel_)
        {
            titel.format(format);
            nana::size sz = titel.measure(1000);
            std::stringstream ft;
            ft << "vertical margin=[0," << gap << "," << gap << "," << gap << "]"
               << " <weight="  << sz.height  << " <weight=5> <titel weight=" << sz.width+1  << "> >";
            fmt = ft.str();
            plc["titel"] << titel;
            drawing dw(*this);
		    dw.draw([gap,sz](paint::graphics& graph)
		    {
			    graph.rectangle(rectangle(gap-1, sz.height/2, graph.width()-2*(gap-1), graph.height()-sz.height/2-(gap-1)),false, colors::gray_border);
		    });
            plc.div(fmt.c_str());

        }
    };
}

int main()
{
	form fm;
    place plc(fm);


	dm::group grp;
	grp.create(fm );
    grp.create_widget<label>(STR("Group:"));

    plc["left"] << grp;

    dm::group1 grp1(fm, STR("A new <bold=true, color=0xff0000, font=\"Consolas\">Group:</>"), true );
    button b1{grp1, STR("add button")};
    button b2{grp1, STR("button2")};
    button b3{grp1, STR("button3")};
    grp1.fmt += "<vertical margin=2 gap= 2 < <left> | 70% <right>> >";
    grp1.plc["left"] << b1  ;
    grp1.plc["right"] << b2 << b3;
    grp1.plc.div(grp1.fmt.c_str());
    //grp1.plc.collocate();    // problem !!!!!!!!!!!!!!

    plc["right"] << grp1;
    plc.div("horizontal gap=3 margin=30  < <left> | 70% <right>> ");
    plc.collocate();
    grp1.plc.collocate();    // OK

	b1.events().click([&grp]
	{
		auto p = grp.create_widget<button>();
		p->caption(L"Button");
	});

	fm.show();

	::nana::exec();
}
/// 