#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <iostream> 
int main()
{
	using namespace nana::gui;	   //No Minimize button, no maximize button and sizable frame.
	form fm(API::make_center(240, 100), appear::decorate<>());
	fm.caption(STR("Nana C++ Library - Example"));
	combox child ( fm, nana::rectangle ( 20, 3, 150, 30) );
	child.push_back(STR("Item 1"));
	child.push_back(STR("Item 2"));
	child.push_back(STR("Item 3"));
	child.push_back(STR("Item 4"));
	child.push_back(STR("Item 5"));
				// This function will be called after changing the combox text by selecting 
				// a text from the drop-down list.
	child.ext_event().selected = [](combox&){ std::cout<<"selected a new text"<<std::endl; };
	fm.show();
	exec();
}
