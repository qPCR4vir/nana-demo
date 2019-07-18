#include <nana/gui.hpp> 
int main() 
{ 
    using namespace nana; 
    form fm(API::make_center(240, 160),  appear::decorate<appear::minimize, appear::maximize>()); 
    fm.show(); 
    exec();
} 
