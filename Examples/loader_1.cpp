#include <nana/gui.hpp> 

int main() 
{ 
    using namespace nana; 
    form_loader<form>()().show(); 
    exec(); 
} 