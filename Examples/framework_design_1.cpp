#include <nana/gui.hpp> 
#include <nana/gui/widgets/button.hpp> 
#include <nana/gui/place.hpp> 
#include <iostream> 


struct data { std::string url; }; 

class uiface 
{ public: 
    virtual ~uiface() = 0; 
    virtual void create_ui_element(const data&) = 0; 
}; 

uiface::~uiface(){} 

class framework 
{ 
    uiface            *uiface_{}; 
    std::vector<data>  cont_; 

public: 
    framework()
    { 
        data dat; 
        dat.url = "http://nanapro.org/"; 
        cont_.push_back(dat); 

        dat.url = "nanaproject.wordpress.com"; 
        cont_.push_back(dat); 
    } 
    void set(uiface * uif) { uiface_ = uif; } 
    void work() 
    { 
        if(uiface_) 
          for(auto & ui_el : cont_ ) 
              uiface_->create_ui_element(ui_el); 
    } 
}; 

namespace ui 
{ 
    using namespace nana; 

    class bar : public form, public uiface 
    { 
        using btn_pair = std::pair<std::shared_ptr<button>, data> ; 

        place plc_{*this}; 
        std::vector<btn_pair> ui_el_; 
     
     public: 
        bar() {  plc_.div("<vertical abc gap=3>");} 

      private: 
               //Now we implement the virtual function declared by uiface 
        void create_ui_element(const data& dat) override
        { 
            btn_pair p{ std::make_shared<button>(*this) , dat} ; 

                          //Make the click event 
            p.first->events().click(  [&](const arg_click& ei){_m_click( ei);} );
            p.first->caption( dat.url) ; 
  
            plc_["abc"]<< *(p.first); 
            plc_.collocate();

            ui_el_.push_back(p); 
        } 
        void _m_click(const arg_click& ei)
        { 
            //Check which button is clicked 
            for(btn_pair &bp : ui_el_) 
            { if(bp.first->handle() == ei.window_handle) 
              {     //Show data bp.second; 
                std::cout<<"open "<<bp.second.url<<std::endl; 
                break; 
              } 
            } 
        } 

    }; 
} 

int main() 
{ 
    ui::bar bar; 
    bar.show(); 
    framework fw; 
    fw.set(&bar); 
    fw.work(); 
    nana::exec();
} 