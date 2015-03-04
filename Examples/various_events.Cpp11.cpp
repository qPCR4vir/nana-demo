#include <nana/gui/wvl.hpp> 
#include <iostream> 
void foo() 
{ 
    std::system("cls"); 
    std::cout<<"foo"<<std::endl; 
} 
void foo_with_eventinfo(const nana::gui::eventinfo& ei) 
{ 
  std::cout << "foo_with_eventinfo, mouse pos = ("
   << ei.mouse.x  << ", "  <<  ei.mouse.y  << ")" << std::endl; 
} 
class click_stat 
{ public: 
    click_stat(): n_(0) {} 
    void respond   ()   { std::cout<<"click_stat = "<<++n_<<std::endl; } 
    void respond_ei(const nana::gui::eventinfo& ei) 
    { 
         std::cout << "click_state width eventinfo = " << n_ 
        << ", mouse pos = ("  <<ei.mouse.x<<", "<<ei.mouse.y<<")"
        << std::endl; 
    } 
 private: 
   int n_; 
}; 
///   \todo dont compile
int main() 
{ 
    using namespace nana::gui; 
    typedef std::function<void()>                   fun_t; 
    typedef std::function<void(const eventinfo&)>   fun_with_param_t; 
    form        fm; 
    click_stat  cs; 
    fun_t       f(foo); 
    fm.make_event<events::click>(f); 
    f = fun_t(cs, &click_stat::respond); 
    fm.make_event<events::click>(f); 
    fun_with_param_t fp(foo_with_eventinfo); 
    fm.make_event<events::click>(fp); 
    fp = fun_with_param_t(cs, &click_stat::respond_ei); 
    fm.make_event<events::click>(fp); 
    fm.show(); 
    exec(); 
} 