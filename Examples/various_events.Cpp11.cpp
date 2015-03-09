    #include <nana/gui/wvl.hpp> 
    #include <iostream> 
    void foo() 
    { 
        std::system("cls"); 
        std::cout<<"foo"<<std::endl; 
    } 
    void foo_with_eventinfo(const nana::arg_mouse& ei) 
    { 
      std::cout << "foo_with_eventinfo, mouse pos = ("
       << ei.pos.x  << ", "  <<  ei.pos.y  << ")" << std::endl; 
    } 
    class click_stat 
    { public: 
        click_stat(): n_(0) {} 
        void respond   ()   { std::cout<<"click_stat = "<<++n_<<std::endl; } 
        void respond_ei(const nana::arg_mouse& ei) 
        { 
             std::cout << "click_state width eventinfo = " << n_ 
            << ", mouse pos = ("  <<ei.pos.x<<", "<<ei.pos.y<<")"
            << std::endl; 
        } 
     private: 
       int n_; 
    }; 

    int main() 
    { 
        using namespace nana; 
        using fun_t            = std::function<void(                )>  ; 
        using fun_with_param_t = std::function<void(const arg_mouse&)>  ; 

        form        fm; 
        click_stat  cs; 
    
        fun_t       f=foo; 
        fm.events().click(f); 

        fun_with_param_t fp(foo_with_eventinfo); 
        fm.events().click(fp); 

        f= std::bind( &click_stat::respond, cs); 
        fm.events().click(f ); 

        fp= std::bind( &click_stat::respond_ei, cs , std::placeholders::_1 );
        fm.events().click(fp); 

        fm.show(); 
        exec(); 
    } 