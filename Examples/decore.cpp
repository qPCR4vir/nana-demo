#include <nana/gui/wvl.hpp> 
int main() 
{ 
    using namespace nana::gui; 
    form fm(API::make_center(240, 160), 
    appear::decorate<appear::minimize, appear::maximize>()); 
    fm.show(); 
    exec(); 
} 