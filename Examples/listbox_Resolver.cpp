#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <iostream>

struct person
{
    std::string name;
    bool         gender;
    unsigned     age;
};

nana::listbox::oresolver& operator << ( nana::listbox::oresolver& or , const person& pr)
{
	or << pr.name;
	or << (pr.gender ?  ("Male") :  ("Female"));
	or << pr.age;  
	return or ;
}
std::ostream & operator << (std::ostream& or , const person& pr)
{
	or << pr.name;
	or << (pr.gender ?  ("Male") :  ("Female"));
	or << pr.age;
	return or ;
}
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& or , person& pr)
{
	or >> pr.name;
	std::string gender;
	or >> gender;
	pr.gender = gender ==  ("Male") ;
	or >> pr.age;  
	return or ;
}



int main()
{
	using namespace nana;

	form fm;
	listbox lb(fm, nana::rectangle(10, 10, 280, 120));

	auto cat = lb.at(0);
	if (cat.begin() == cat.end())
		std::wcout <<  ("true ");
	else 		
		std::wcout <<  ("false ");

	std::wcout << "\n";

	lb.append_header( ("Name"), 100);
	lb.append_header( ("Gender"), 60);
	lb.append_header( ("Age"), 60);

	//lb.resolver(resolver()); //Pass the user defined resolver to the listbox

	person pr;
	pr.name =  ("Peter");
	pr.gender = true;
	pr.age = 10;
	lb.at(0).append (pr); //person to item. resolver::decode() is required

	person prr;
	lb.at(0).at(0).resolve_to( prr); //item to person. resolver::encode() is required
	std::cout << pr << "\n";
	std::cout << prr << "\n";
	fm.show();
	exec();
}