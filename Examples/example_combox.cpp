#include <nana/gui.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <iostream> 
int main()
{
	using namespace nana;	   //No Minimize button, no maximize button and sizable frame.
	form fm(API::make_center(240, 100), appear::decorate<>());
	fm.caption(("Nana C++ Library - Example"));
	combox child ( fm, nana::rectangle ( 20, 3, 150, 30) );
	child.push_back("Item 1");
	child.push_back("Item 2");
	child.push_back("Item 3");
	child.push_back("Item 4");
	child.push_back("Item 5");
				// This function will be called after changing the combox text by selecting 
				// a text from the drop-down list.
       // dont use the arg info
	//child.events().selected ( []( ){ std::cout<<"selected a new text"<<std::endl; });

       // use the arg info
	child.events().selected ( [](const arg_combox &ar_cbx){ std::cout<<ar_cbx.widget.caption()<<std::endl; });
	fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&child]()
	{
		click(child);
		child.option(3);
		child.events().selected.emit(arg_combox{ child }, child);

	}
#endif

	);
}
