
// http://nanapro.org/en-us/thread.php?id=74f0972e5806733fac93bd659dfdaba9&stmp=1445985332412

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/form.hpp>
//#include <nana/gui/widgets/group.hpp>
//#include <nana/gui/widgets/label.hpp>

int main()
{
	using namespace nana;

	form fm;

	place plc(fm);
	plc.div("vert <abc margin=5 weight=30>|<dock <bottom pane1><pane2><pane3 top><pane4>>");

	button btn(fm);
	btn.caption("Button");

	btn.events().click([&plc]
	{
		//re-div
		plc.div("vert <abc margin=5 weight=30>|<dock <bottom pane1><pane3 top><pane4><pane2>>");
		plc.collocate();
	});

	plc["abc"] << btn;

	//sets factories for panes
	//there are 3 parameters
	//	pane name, specified the pane where the button is created in.
	//	factory name, a name is used to create the pane
	//	the third parameter and after the thired, are passed to the constructor of button. button(PaneHandle, std::forward<Args>(third_args)...);
	plc.dock<button>("pane1", "f1", std::string("Button1"));
	plc.dock<button>("pane2", "f2", std::string("Button2"));
	plc.dock<button>("pane3", "f3", std::string("Button3"));
	plc.dock<button>("pane4", "f4", std::string("Button4"));

	//Call the factory to create the dockpane
	plc.dock_create("f1");
	plc.dock_create("f2");
	plc.dock_create("f3");
	plc.dock_create("f4");

	plc.collocate();

	fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&btn]() {click(btn); }
#endif

	);
}
