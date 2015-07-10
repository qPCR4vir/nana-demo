#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <iostream>

struct person
{
    nana::string name;
    bool         gender;
    unsigned     age;
};

nana::listbox::oresolver& operator << ( nana::listbox::oresolver& or , const person& pr)
{
	or << pr.name;
	or << (pr.gender ? STR("Male") : STR("Female"));
	or << pr.age;  
	return or ;
}
std::wostream & operator << (std::wostream& or , const person& pr)
{
	or << pr.name;
	or << (pr.gender ? STR("Male") : STR("Female"));
	or << pr.age;
	return or ;
}
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& or , person& pr)
{
	or >> pr.name;
	nana::string gender;
	or >> gender;
	pr.gender = gender == STR("Male") ;
	or >> pr.age;  
	return or ;
}



int main()
{
	using namespace nana;

	form fm;
	listbox lb/*(fm, nana::rectangle(10, 10, 280, 120))*/;

	auto cat = lb.at(0);
	if (cat.begin() == cat.end())
		std::wcout << STR("true ");
	else 		
		std::wcout << STR("false ");

	std::wcout << "\n";

	lb.append_header(STR("Name"), 100);
	lb.append_header(STR("Gender"), 60);
	lb.append_header(STR("Age"), 60);

	//lb.resolver(resolver()); //Pass the user defined resolver to the listbox

	person pr;
	pr.name = STR("Peter");
	pr.gender = true;
	pr.age = 10;
	lb.at(0).append (pr); //person to item. resolver::decode() is required

	person prr;
	lb.at(0).at(0).resolve_to( prr); //item to person. resolver::encode() is required
	std::wcout << pr << "\n";
	std::wcout << prr << "\n";
	fm.show();
	exec();
}