
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
			    graph.rectangle(false, colors::black);
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

        group1(widget &owner, ::nana::string titel={}, rectangle r={})
            : panel  (owner, r), 
              titel  (*this, titel),
              fmt    ("vertical margin=[0,3,3,3] <weight=18 <weight=5> <titel weight=43> >")
        {
            plc["titel"] << titel;
            drawing dw(*this);
		    dw.draw([](paint::graphics& graph)
		    {
			    graph.rectangle(false, colors::black);
		    });
            plc.div(fmt.c_str());

        }
    };
}

int main()
{
	form fm(rectangle{ 10, 10, 600, 300 });
    place plc(fm);


	dm::group grp;
	grp.create(fm );
    grp.create_widget<label>(STR("Group:"));

    plc["abc"] << grp;

    dm::group1 grp1(fm, STR("Group1:"), rectangle{ 220, 10, 200, 200 });
    button b1{grp1, STR("add button")};
    button b2{grp1, STR("button2")};
    button b3{grp1, STR("button3")};
    grp1.fmt += "<vertical margin=2 gap= 2 buttons>";
    grp1.plc["buttons"] << b1 << b2 << b3;
    grp1.plc.div(grp1.fmt.c_str());
    grp1.plc.collocate();

    plc["abc"] << grp1;
    plc.div("horizontal gap=3 margin=30 abc");
    plc.collocate();

	b1.events().click([&grp]
	{
		auto p = grp.create_widget<button>();
		p->caption(L"Button");
	});

	fm.show();

	::nana::exec();
}