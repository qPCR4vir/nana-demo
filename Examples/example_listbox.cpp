	#include <nana/gui/wvl.hpp>
	#include <nana/gui/widgets/listbox.hpp>
		int main()
		{
		   using namespace nana::gui;
		   form fm;
		   listbox lb(fm, nana::rectangle(10, 10, 280, 120));
		   lb.append_header(STR("Header"), 200);
		   lb.at(0).append(STR("int"));
		   lb.at(0).append(STR("double"));

		   lb.anyobj(0, 0, 10);     ///    \todo this crash   lb.anyobj(0, 0, 10);   

		   lb.anyobj(0, 1, 0.1);   ///    \todo this crash  

		   int * pi = lb.anyobj<int>(0, 0); 	  // it returns a nullptr if there is not an int object specified.
		   double * pd = lb.anyobj<double>(0, 1); // it returns a nullptr if there is not an double object specified.

		   fm.show();
		   exec();
		 }