#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>

#include <iostream> 

void clicked(const nana::arg_click & eventinfo)
{
     std::cout<<  "When the window  fm  is clicked, this function is called. \n";
}



int main()
{
    using namespace nana;
    form fm;
    fm.events().click(clicked);
    fm.show();
	exec(10, [&fm]() 
		{
			std::cout << "3 times automatic click. \n";
			fm.events().click.emit(nana::arg_click{});
			fm.events().click.emit(nana::arg_click{});
			fm.events().click.emit(nana::arg_click{});
			// char c;
			// std::cin >> c;

			fm.close(); 
	     });
}