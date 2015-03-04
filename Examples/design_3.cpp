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
{ 
  public: 
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

    using namespace nana::gui; 


namespace ui 
{ 
    using namespace nana::gui; 

class bar : public form, public uiface 
{   
  public: 
    bar() { gird_.bind(*this); } 

  private: 
                         //Now we implement the virtual function declared by uiface 
	virtual void create_ui_element(const data& dat) 
	{ 
		std::shared_ptr<button> p(new button(*this)); 
		 //Make the click event 
		typedef std::function<void()> fun; //or std::function<void()> 
		p->make_event<events::click> ( fun (std::bind( &bar::_m_click, this, dat) )); 
		p->caption(nana::charset(dat.url)); 
		gird_.push(*p, 0, 22); 
		ui_el_.push_back(p); 
	} 
	void _m_click(const data& dat) 
	{ 
	   std::cout<<"open "<<dat.url<<std::endl; 
	} 

private: 
    gird gird_; 
    std::vector<std::shared_ptr<button> > ui_el_; 
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






