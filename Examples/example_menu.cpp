		#include <nana/gui/wvl.hpp>
		#include <nana/gui/widgets/button.hpp>
		#include <nana/gui/widgets/menu.hpp>

		void on_menu_item(nana::gui::menu::item_proxy& ip)
		{
			 std::size_t index = ip.index(); //Get the index of the clicking item.
		}

		int main()
		{
			using namespace nana::gui;
			form fm;

			;//We need a menu object
			menu mobj;
			mobj.append ( STR("Item 0"), &on_menu_item);
			mobj.append_splitter();
			mobj.append ( STR("Item 1"), &on_menu_item);

			;//Now we need a button.
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
