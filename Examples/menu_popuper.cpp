#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/menu.hpp>

using namespace nana::gui;

//class menu_popuper
//{
//	public:
//		//C++03
//	menu_popuper(menu&, mouse bttn= mouse::right_button);
//	menu_popuper(menu&, window owner, const nana::point& pos, mouse bttn=  mouse::right_button	);
//
//		//C++11
//	menu_popuper(menu&, mouse = mouse::right_button);
//	menu_popuper(menu&, window owner, const nana::point& pos, mouse bttn= mouse::right_button);
//
//	void operator()(const eventinfo&);
//		
//private: 			//Implemented-Specified private members
//		
//};

// Now let's use it. There is a button, it popups the menu when it is clicked.

void on_menu_item(nana::gui::menu::item_proxy& ip)
{
		std::size_t index = ip.index(); //Get the index of the clicking item.
}

int main()
{
	form fm;

		//We need a menu object
	menu mobj;
	mobj.append ( STR("Item 0"), &on_menu_item);
	mobj.append_splitter();
	mobj.append ( STR("Item 1"), &on_menu_item);

		//Now we need a button.
	button btn(fm, nana::rectangle(nana::point(10, 10), nana::size(100, 25)));
	btn.caption(STR("Popup Menu"));

	;//Popup the menu when right clicking the button.
	btn.make_event<events::click>(menu_popuper(mobj));

	;//Or popuping the menu with a specified coordinate when any mouse button is clicked.
	;//btn.make_event<events::click> ( menu_popuper( mobj, btn, nana::point(0, 26),
	;//                                mouse::any_button );

	fm.show();
	exec();
}

