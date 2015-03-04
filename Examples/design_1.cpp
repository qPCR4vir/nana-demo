#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/button.hpp> 
#include <nana/gui/layout.hpp> 
#include <iostream> 


struct data { std::string url; }; 

class uiface 
{ public: 
    virtual ~uiface() = 0; 
    virtual void create_ui_element(const data&) = 0; 
}; 

uiface::~uiface(){} 

class framework 
{ public: 
    framework(): uiface_(0) 
    { 
        data dat; 
        dat.url = "stdex.sf.net"; 
        cont_.push_back(dat); 
        dat.url = "nanaproject.wordpress.com"; 
        cont_.push_back(dat); 
    } 
    void set(uiface * uif) { uiface_ = uif; } 
    void work() 
    { if(uiface_) 
      { for(std::vector<data>::const_iterator i = cont_.begin(); i != cont_.end();   ++i) 
        { uiface_->create_ui_element(*i); 
        } 
      } 
    } 
  private: 
    uiface * uiface_; 
    std::vector<data> cont_; 
}; 

namespace ui 
{ 
    using namespace nana::gui; 

    class bar : public form, public uiface 
    { public: 
        bar() { gird_.bind(*this); } 
      private: 
        typedef std::pair<std::shared_ptr<button>, data> btn_pair; 
               //Now we implement the virtual function that declared by uiface 
        virtual void create_ui_element(const data& dat) 
        { 
            btn_pair p(std::shared_ptr<button>(new button(*this)), dat); 
                          //Make the click event 
            p.first->make_event<events::click>(*this, &bar::_m_click); 
            p.first->caption(nana::charset(dat.url)); 
            gird_.push(*(p.first), 0, 22); 
            ui_el_.push_back(p); 
        } 
      private: 
        void _m_click(const eventinfo& ei) 
        { 
            //Check which button is clicked 
            for(std::vector<btn_pair>::iterator i = ui_el_.begin(); i != ui_el_.end();++i) 
            { if(i->first->handle() == ei.window) 
              {     //Show data 
                i->second; 
                std::cout<<"open "<<i->second.url<<std::endl; 
                break; 
              } 
            } 
        } 
     private: 
        gird gird_; 
        std::vector<btn_pair> ui_el_; 
    }; 
} 

int main() 
{ 
ui::bar bar; 
bar.show(); 
framework fw; 
fw.set(&bar); 
fw.work(); 
nana::gui::exec(); 
} 