/*
 *	This is a demo of Nana C++ Library
 *	Author: Jinhao, simplified by qPCR4vir
 *	The demo requires Nana 1.0 and C++11 compiler
 *	Screenshot at http://sourceforge.net/projects/stdex
 */
#include <memory>
#include <vector>
#include <map>

#include <nana/deploy.hpp>
#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/filesystem/filesystem_ext.hpp>


namespace demo
{
	using namespace nana;
	namespace fs = std::filesystem;
	namespace fs_ext = nana::filesystem_ext;

	class tab_page_listbox 	: public panel<false>
	{
		place       place_      {*this} ;
		listbox		listbox_    {*this} ;
		checkbox	checkbox_   {*this, "Checkable Listbox"} ;
	public:
		tab_page_listbox(window wd)	: panel<false>(wd)
		{
			place_.div("< <list> |30% <check margin=5> >");

			listbox_.append_header("Supported compilers", 200);        // A new column  --> listbx.headers().emplace_back()
			listbox_.append_header("Version", 100);                    // Another column

			auto cat=listbox_.append("Nana.C++03 [abandoned]");        // cat_proxy, A new category --> listbx.categories().emplace_back(), or shortcut: listb.emplace_back() : a listbox is, among others, a container of categories)
            cat.append( {"GCC  "        , " 3.4 and later"  });        // A new item. cat.items().emplace_back(), or shortcut: cat.emplace_back(): a categiry is, among others, a container of items
            cat.append( {"Visual C++  " , " 2003 and later" });        // Another item

			cat=listbox_.append("Nana.C++11");                         // Another category
            cat.append( {"GCC  "," 4.6 and later" });                  // A new item
            cat.append( {"Visual C++  ", " 2013 and later" });         // Another item
            cat.append( {"Clang  " , " ?" });                          // Another item

			cat=listbox_.append("Nana.C++14 :-)");                     // Another category
            cat.append( {"GCC  ", " 4.8? and later" });                // A new item
            cat.append( {"Visual C++  ", " 2015 and later" });         // Another item
            cat.append( {"Clang  ", " ?" });                           // Another item

			cat=listbox_.append("Nana.C++17 :-)");                      // Another category
            cat.append( {"GCC  ", " 6? and later" });                   // A new item
            cat.append( {"Visual C++  ", " 2015 Update 2 and later" }); // Another item
            cat.append( {"Clang  ", " ?" });                            // Another item

            checkbox_.events().click([this](){ listbox_.checkable(checkbox_.checked()); });

			place_.field("list")<<listbox_; 
            place_["check"]<<checkbox_;
		}
	};

	class tab_page_treebox 		: public panel<false>
	{
		place   place_   {*this};
		treebox treebox_ {*this};
	public:
		typedef treebox::item_proxy item_proxy;

		tab_page_treebox(window wd) 
			: panel<false>(wd)
		{
			place_.div("<tree>");
			place_["tree"]<<treebox_;

			item_proxy root_node = treebox_.insert(fs_ext::def_root, fs_ext::def_rootname);

			// find first directory --> use std::find ?
			// Boost can throw an exception "Access is denied"
			// when accessing some system paths, like "C:\Config.Msi"
		try {
		      for (const auto& dir : fs::directory_iterator{ fs_ext::def_rootstr })

			{
				if (!fs::is_directory(dir)) continue;
				std::string fname = dir.path().filename().generic_string();
				treebox_.insert(root_node, fname, fname);
				break;
			}
		} catch (...) {}

            treebox_.events().expanded([this](const arg_treebox& a){_m_expand(a.widget, a.item, a.operated);});
		}
	private:
		void _m_expand(nana::window, item_proxy node, bool exp)
		{
			if(!exp) return; //If this is contracted.

			std::string path = treebox_.make_key_path(node, "/") + "/";
			//Trim the head of whitespace, more portable, because the root
			//under Linux is a whitespace
			std::string::size_type path_start_pos = path.find_first_not_of(" ");
			if(path_start_pos != std::string::npos)
				path.erase(0, path_start_pos);
			//avoids frequent useless refreshing
			treebox_.auto_draw(false);

			//Walk in the path directory for sub directories.
		try {		
			for (const auto& dir : fs::directory_iterator{ path })
			{
				if (!fs::is_directory(dir)) continue;
				std::string fname = dir.path().filename().generic_string();
				auto child = treebox_.insert(node, fname, fname);
				if (child.empty()) continue;

				try {
				//Find a directory in child directory, if there is a directory,
				//insert it into the child, just insert one node to indicate the
				//node has a child and an arrow symbol will be displayed in the
				//front of the node.   use std::find_first ??
				for (const auto& sdir : fs::directory_iterator{ dir.path() })
				{
					if (!fs::is_directory(sdir)) continue; //If it is not a directory.
					
					std::string fname = sdir.path().filename().generic_string();
					treebox_.insert(child, fname, fname);
					break;
				}
				} catch (...) {}
			}
			} catch (...) {}
			treebox_.auto_draw(true);
		}
	};

	class tab_page_datechooser		: public panel<false>
	{
		date_chooser  date_    { *this, nana::rectangle(10, 10, 260, 200) };
		textbox	      textbox_ { *this, nana::rectangle(280, 10, 170, 23) };
	 public:
		tab_page_datechooser(window wd)
			: panel<false>(wd)
		{
			textbox_.tip_string("Input a date:");

            date_.events().dbl_click([this]()
            {
                auto dt=date_.read().read();
                textbox_.reset(charset(std::to_string(dt.year) + "-" + std::to_string(dt.month)+ "-" + std::to_string(dt.day)));
            });
		}
	};
	
	class tab_page_radiogroup 	: public panel<false>
	{
		place           place_     { *this };
		radio_group     group_  ;     //  use a group ??
		group           rgr_       { *this, "Manufacturers" };
		label           label_     { *this, "Select an airplane manufacturer" };
		categorize<int> categorize_{ *this };
		std::vector<std::shared_ptr<checkbox>> box_;
	public:
		tab_page_radiogroup(window wd)
			:	panel<false>(wd)
		{
			place_.div(R"(
                          <weight=5><vertical < weight=5 >
                                              < weight=150 <check   gap=5     vertical> <real_gr margin=10 max=130> >
                                              < weight=50  gap=5 bottom_cat vertical>   >
                          <weight=5>    
                         )");
			rgr_.radio_mode(true);
            place_["real_gr"]<<rgr_ ;
			const std::string str[6] = { "Airbus", "AHTOHOB",
					                     "Boeing", "Bombardier",
                                         "Cessna", "EMBRAER"     };
			std::map<std::string, std::vector<std::string>> airplanes
			{
				{ "Airbus"    ,{ "320" , "330" } },
				{ "AHTOHOB"   ,{ "An-124" , "An-225" } },
				{ "Boeing"    ,{ "737", "747", "757", "767", "777", "787" } },
				{ "Bombardier",{ "CRJ" , "Dash 8" } },
				{ "Cessna"    ,{ "C-170" , "C-172" } },
				{ "EMBRAER"   ,{ "ERJ-145" , "E-195" } }
			};

			for(const auto& airplane : airplanes)
			{
				rgr_.add_option(airplane.first).events().checked([this](const arg_checkbox& msg) {
							std::string str = msg.widget->caption()   ;
							this->label_.caption( std::string("You have selected ") + str);
							this->categorize_.caption(std::string("Manufacturer\\") + str);
				      });

				auto p = std::make_shared<checkbox>(*this);
				box_.push_back(p);


				//Add the checkbox to the radio group. The radio group does not
				//manage the life of checkboxs.
				group_.add(*p);
				place_["check"]<< *p ;

				p->caption(airplane.first);
				p->events().click([this]()
					{
						std::size_t index = this->group_.checked();
						std::string str = this->box_[index]->caption();

						this->label_.caption( std::string("You have selected ") + str);
						this->categorize_.caption(std::string("Manufacturer\\") + str);
					});	
			}

			place_["bottom_cat"] << label_ << categorize_ ;

			for(auto & mstr: airplanes)
			{
				categorize_.caption("Manufacturer");
				categorize_.insert(mstr.first, 0);
				for(auto & astr : mstr.second)
					categorize_.childset(astr, 0);
			}
		}
	};

	class widget_show	: public form
	{
        place        place_ {*this} ;    //A layout management
		timer        timer_ ;
		group        simple_    {*this,  "Simple <bold=true, color=0xff0000>widgets</>", true /*formated*/} ,
                     buttons_   {simple_,  "Buttons"},
				     comboxes_  {simple_,  "Comboxes"},
				     labels_    {simple_,  "Labels"},
				     progreses_ {simple_,  "Progres bars"};
        button       b_n    {buttons_,  ("Normal Button")} ,
                     b_i    {buttons_,  ("Button with An Image")} ,
                     b_p    {buttons_,  ("Pushed Button")} ;
		combox       cb_e   {comboxes_,  ("Normal combox")} ,
                     cb_u   {comboxes_,  ("This is an uneditable combox")} ;
		label      	 lab    {labels_};
		progress     progr_k{progreses_},
                     progr_u{progreses_} ;
		tabbar<std::string> tabbar_{*this} ;
        tab_page_listbox     tp_l  {*this} ;
        tab_page_treebox     tp_t  {*this} ;
        tab_page_datechooser tp_d  {*this} ;
        tab_page_radiogroup  tp_r  {*this} ;

		std::vector<std::shared_ptr<panel<false>>> tabpages_;

	public:
		widget_show()
			: form(API::make_center(500, 600), appear::decorate<appear::sizable>())
		{
            nana::API::track_window_size(*this, {300,300}, false); //minimum
		    this->caption(("This is a demo of Nana C++ Library"));
			place_.div( R"(vertical
                                   <weight=30% min=260 <weight=10> <simples gap=3 margin=5> >
                                   <weight=20 tab >
                                   <tab_frame>        )"      );

			place_["simples"] << simple_;
			simple_.div("vertical all   min=260 gap=3 margin=5");
			simple_["all"]<< buttons_ << comboxes_ << labels_ << progreses_ ;


			_m_init_buttons();
			_m_init_comboxs();
			_m_init_labels();
			_m_init_progresses();
			_m_init_tabbar();

			this->events().unload([this](const arg_unload& ei)
				{
					msgbox mb(*this,  ("Question"), msgbox::yes_no);
					mb.icon(mb.icon_question) <<  ("Are you sure you want to exit the demo?");
					ei.cancel = ( mb() == mb.pick_no );
				});

			place_.collocate();
		};
	private:
		void _m_init_buttons()
		{
			buttons_.div("buttons min=25 gap=5 margin=3");
			buttons_["buttons"] << b_n << b_i << b_p ;

			msgbox mb{ *this,  "Msgbox" };
			mb.icon(mb.icon_information) << "Button Clicked";

			b_n.events().click(mb);
			b_i.events().click(mb);
			b_p.events().click(mb);

			b_i.icon( paint::image("../Examples/image.bmp"));

			b_p.enable_pushed(true);
		}

		void _m_init_comboxs()
		{
			comboxes_.div("buttons  min=25 gap=5 margin=3");
			comboxes_["buttons"] << cb_e << cb_u ;

			cb_e.push_back( ("Item 0"));
			cb_e.push_back( ("Item 1"));
			cb_u.push_back( ("Item 0"));
			cb_u.push_back( ("Item 1"));

			msgbox mb(*this,  ("Item Selected"));
			mb.icon(mb.icon_information);

			cb_e.editable(true);
			cb_e.events().selected( [this, mb](const nana::arg_combox& acmb) mutable
			{
				mb << ("The item ") << acmb.widget.option() << (" is selected in editable combox");
				mb();
				//Clear the buffer, otherwise the mb shows the text generated in
				//the last selected event.
				mb.clear();
			});

			cb_u.events().selected ( [this, mb](const nana::arg_combox& acmb) mutable
			{
				mb << ("The item ") << acmb.widget.option() <<  (" is selected in uneditable combox");
				mb();
				mb.clear();
			});
		}

		void _m_init_labels()
		{
			labels_.div("buttons gap=5 min=25 margin=3");
			labels_["buttons"] <<  ("This is a normal label") << lab;  //			place_.field("labels") <<  ("This is a normal label") << lab;

			lab.format(true);
			lab.caption( R"(This is a <bold, color=0xFF0000, font="Consolas">formatted label</>)");
		}

		void _m_init_progresses()
		{
			progreses_.div("buttons gap=5 min=25 margin=3");
			progreses_["buttons"] <<  progr_u << progr_k;    //	place_.field("progresses")<< progr_u << progr_k;

            //One progress is unknown mode, the other is known mode.
			progr_u.tooltip( ("Unknwon in progress"));
            progr_u.unknown(true);	
            
			progr_k.tooltip( ("Knwon in progress"));
			progr_k.unknown(false);	 

			timer_.elapse([this](const nana::arg_elapse& a)
			{
			    //Resets the known mode progress if its value reaches the amount value.
                if(progr_k.value() == progr_k.amount())
							progr_k.value(0);

				progr_k.inc();
				progr_u.inc();
			});

			timer_.interval(std::chrono::milliseconds{80});
			timer_.start();
		}

		void _m_init_tabbar()
		{
			place_.field("tab") << tabbar_;
			place_.field("tab_frame").fasten(tp_l)	//Fasten the tab pages
                                     .fasten(tp_t)
                                     .fasten(tp_d)
                                     .fasten(tp_r);

			tabbar_.append ("listbox",      tp_l)
				   .append ("treebox",      tp_t)
				   .append ("date chooser", tp_d)
				   .append ("radio group",      tp_r);
		}
	};//end class widget_show
	
	void go()
	{
		widget_show wdshow;
		wdshow.show();
		exec();
	}
}

int main()
{
	demo::go();	
}
