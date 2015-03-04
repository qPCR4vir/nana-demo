	#include <iostream>
	#include <nana/gui/wvl.hpp>
	#include <nana/gui/widgets/categorize.hpp>

		class renderer
		{
		 public:
			typedef nana::paint::graphics & graph_reference;
			enum state_t {state_normal, state_over, state_pressed};
			struct ui_element
			{
				enum t
				{
					none, 		//Out of the widget
					somewhere, item_root, item_name, item_arrow
				};
				static const size_t npos = static_cast<size_t>(-1);
				t what;
				size_t index;
				ui_element();
			};
			virtual ~renderer() = 0;
			virtual void background(graph_reference, nana::gui::window, const nana::rectangle&, 
									const   ui_element&)     = 0;
			virtual void root_arrow(graph_reference, const nana::rectangle&, state_t) = 0;
			virtual void item(graph_reference, const nana::rectangle&, size_t index,
			const nana::string& name, unsigned text_height,
			bool has_child, state_t) = 0;
			virtual void border(graph_reference) = 0;
		};




	void selected(nana::gui::categorize<int> & categ, int&)
	{
		nana::gui::msgbox mb(categ, STR("categorize"));
		mb.icon(mb.icon_information);
		mb<<STR("The value of selected is ")<<categ.value()<<STR(", caption is \"")
		<<categ.caption()<<STR("\""); //caption() method, see Note 3.
		mb();
	}

	int main()
	{
		using namespace nana::gui;
		form fm;
		categorize<int> categ(fm, nana::rectangle(10, 10, 200, 24));

		categ.childset(STR("Invalid Category"), 0); // Invalid category because of empty current category.

		categ.insert(STR("First"), 0); 	// Insert a category and now it is the current category.

		categ.insert(STR("Second"), 1); // Insert a category as a child of "First" category, and then 
		                              ;  // the "Second" is the current category.

		categ.childset(STR("Third"), 2);
		; // Insert a category as a child of "Second".

		categ.childset(STR("Another Child"), 2);
			; // childset() always insert categories into current category, and
			; // it does not displace the current category, and therefore "Another Child"
			; // is a child of "Second".

		categ.ext_event().selected = selected;
			; // This may crash in VC2012-RC due to compiler bug.
			; // Please refer to compatibility-issue for more details

		fm.show();
		exec();
	}