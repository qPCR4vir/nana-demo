/*
 *	This is a demo of Nana C++ Library
 *	Author: Jinhao
 *	The demo requires Nana 0.6 and C++11 compiler
 *	Screenshot at http://sourceforge.net/projects/stdex
 */
#include <nana/gui/wvl.hpp>
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
#include <nana/filesystem/file_iterator.hpp>
#include <nana/gui/widgets/date_chooser.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/tooltip.hpp>
#include <memory>
#include <vector>

namespace demo
{
	using namespace nana;

	class tab_page_listbox
		: public panel<false>
	{
	public:
		tab_page_listbox(window wd)
			: panel<false>(wd)
		{
			place_.bind(*this);
			place_.div("< <list> |30% <check margin=5> >");

			listbox_.create(*this);
			listbox_.append_header(STR("Supported compilers"), 200);
			auto cat = listbox_.append(STR("Nana.C++03"));
			cat.push_back(STR("GCC 3.4 and later"));
			cat.push_back(STR("Visual C++ 2003 and later"));

			cat = listbox_.append(STR("Nana.C++11"));
			cat.push_back(STR("GCC 4.6 and later"));
			cat.push_back(STR("Visual C++ 2012 and later"));

			checkbox_.create(*this);
			checkbox_.caption(STR("Checkable Listbox"));
			checkbox_.events().click([this]()
				{
					this->listbox_.checkable(this->checkbox_.checked());
				});

			place_.field("list")<<listbox_; 
            place_["check"]<<checkbox_;
		}
	private:
		place       place_;
		listbox		listbox_;
		checkbox	checkbox_;
	};

	class tab_page_treebox
		: public panel<false>
	{
	public:
		typedef treebox::item_proxy item_proxy;

		tab_page_treebox(window wd)
			: panel<false>(wd)
		{
			place_.bind(*this);
			place_.div("<tree>");
			treebox_.create(*this);
			place_.field("tree")<<treebox_;

#if defined(NANA_WINDOWS)
			item_proxy node = treebox_.insert(STR("C:"), STR("Local Drive(C:)"));
			nana::filesystem::file_iterator i(STR("C:\\")), end;
#elif defined(NANA_LINUX)
			//Use a whitespace for the root key string under Linux
			item_proxy node = treebox_.insert(STR(" "), STR("Root"));
			nana::filesystem::file_iterator i(STR("/")), end;
#endif
			for(; i != end; ++i)
			{
				if(false == i->directory) continue;

				treebox_.insert(node, i->name, i->name);
				break;
			}
            treebox_.events().expanded([this](const arg_treebox& a){_m_expand(a.widget, a.item, a.operated);});
//( [&]( const nana::arg_treebox &tbox_arg_info ) { if (tbox_arg_info.operated) RefreshList(tbox_arg_info.item); });
		}
	private:
		void _m_expand(nana::window, item_proxy node, bool exp)
		{
			if(!exp) return; //If this is contracted.

			nana::string path = treebox_.make_key_path(node, STR("/")) + STR("/");
			//Trim the head of whitespace, more portable, because the root
			//under Linux is a whitespace
			nana::string::size_type path_start_pos = path.find_first_not_of(STR(" "));
			if(path_start_pos != nana::string::npos)
				path.erase(0, path_start_pos);

			//Walk in the path directory for sub directories.
			nana::filesystem::file_iterator i(path), end;
			for(; i != end; ++i)
			{
				if(false == i->directory) continue; //If it is not a directory.

				item_proxy child = treebox_.insert(node, i->name, i->name);
				if(child.empty()) continue;
            
				//Find a directory in child directory, if there is a directory,
				//insert it into the child, just insert one node to indicate the
				//node has a child and an arrow symbol will be displayed in the
				//front of the node.
				nana::filesystem::file_iterator u(path + i->name);
				for(; u != end; ++u)
				{
					if(false == u->directory) continue; //If it is not a directory.
					treebox_.insert(child, u->name, u->name);
					break;
				}
			}
		}
	private:
		place place_;
		treebox treebox_;
	};

	class tab_page_datechooser
		: public panel<false>
	{
	public:
		tab_page_datechooser(window wd)
			: panel<false>(wd)
		{
			date_.create(*this, nana::rectangle(10, 10, 260, 200));
			textbox_.create(*this, nana::rectangle(280, 10, 170, 23));
			textbox_.tip_string(STR("Input a date:"));

            date_.events().dbl_click([this]()
            {
                auto dt=date_.read().read();
                textbox_.reset(charset(std::to_string(dt.year) + "-" + std::to_string(dt.month)+ "-" + std::to_string(dt.day)));
            });
		}
	private:
		date_chooser date_;
		textbox	textbox_;
	};
	
	class tab_page_radiogroup
		: public panel<false>
	{
	public:
		tab_page_radiogroup(window wd)
			:	panel<false>(wd)
		{
			place_.bind(*this);

			place_.div("<weight=5><vertical <weight=5>< weight=150 gap=5 check vertical> <bottom vertical gap=5 weight=50>><weight=5>");

			const nana::string str[6] = {
					STR("Airbus"), STR("AHTOHOB"),
					STR("Boeing"), STR("Bombardier"),
					STR("Cessna"), STR("EMBRAER")};
					
			for(int i = 0; i < 6; ++i)
			{
				auto p = std::make_shared<checkbox>(*this);
				box_.push_back(p);

				//Add the checkbox to the radio group. The radio group does not
				//manage the life of checkboxs.
				group_.add(*p);
				place_.field("check")<< *p ;

				p->caption(str[i]);
				p->events().click([this]()
					{
						std::size_t index = this->group_.checked();
						nana::string str = this->box_[index]->caption();

						this->label_.caption(STR("You have selected ") + str);
						this->categorize_.caption(STR("Manufacturer\\" + str));
					});	
			}
			
			label_.create(*this);
			label_.caption(STR("Select an airplane manufacturer"));

			categorize_.create(*this);

			place_.field("bottom")<< label_ << categorize_ ;

			std::map<nana::string, std::vector<nana::string>> map;
			auto p = &(map[str[0]]);
			p->push_back(STR("320"));
			p->push_back(STR("330"));
			p = &(map[str[1]]);
			p->push_back(STR("An-124"));
			p->push_back(STR("An-225"));
			p = &(map[str[2]]);
			p->push_back(STR("737"));
			p->push_back(STR("747"));
			p->push_back(STR("757"));
			p->push_back(STR("767"));
			p->push_back(STR("777"));
			p->push_back(STR("787"));
			p = &(map[str[3]]);
			p->push_back(STR("CRJ"));
			p->push_back(STR("Dash 8"));
			p = &(map[str[4]]);
			p->push_back(STR("C-170"));
			p->push_back(STR("C-172"));

			p = &(map[str[5]]);
			p->push_back(STR("ERJ-145"));
			p->push_back(STR("E-195"));

			for(auto & mstr: str)
			{
				categorize_.caption(STR("Manufacturer"));
				categorize_.insert(mstr, 0);
				for(auto & astr : map[mstr])
					categorize_.childset(astr, 0);
			}
		}
	private:
		place place_;
		radio_group group_;
		std::vector<std::shared_ptr<checkbox>> box_;
		label label_;
		categorize<int> categorize_;
	};

	class widget_show
		: public form
	{
	public:
		widget_show()
			: form(API::make_center(500, 400), appear::decorate<appear::sizable>())
		{
			this->caption(STR("This is a demo of Nana C++ Library"));
			place_.bind(*this);
			place_.div( "vertical <weight=40% <weight=10><vertical <weight=40 buttons    margin=8 gap=10>"
                        "                                          <weight=40 comboxs    margin=8 gap=10>"
                        "                                          <weight=40 labels     margin=8 gap=10>"
                        "                                          <weight=40 progresses margin=8 gap=10>"
                        "                      > >   "
                        "         <weight=20 tab >   "
                        "         <tab_frame>        "      );

			_m_init_buttons();
			_m_init_comboxs();
			_m_init_labels();
			_m_init_progresses();
			_m_init_tabbar();

			this->events().unload([this](const arg_unload& ei)
				{
					msgbox mb(this->handle(), STR("Question"), msgbox::yes_no);
					mb.icon(mb.icon_question);
					mb<<STR("Are you sure you want to exit the demo?");
					ei.cancel = (mb.pick_no == mb());
				});

			place_.collocate();
		};
	private:
		void _m_init_buttons()
		{

			msgbox mb(*this, STR("Msgbox"));
			mb.icon(mb.icon_information);
			mb<<STR("Button Clicked");
			for(int i = 0; i < 3; ++i)
			{
				auto p = std::make_shared<button>(*this);
				buttons_.push_back(p);
				place_.field("buttons")<<*p; 
				p->events().click(mb);
			}
			
			auto ptr = buttons_[0];
			ptr->caption(STR("Normal Button"));

			ptr = buttons_[1];
			//Nana does not support ICON under Linux now
#if defined(NANA_WINDOWS)
			ptr->icon(STR("image.ico"));
#else
			ptr->icon(STR("image.bmp"));
#endif
			ptr->caption(STR("Button with An Image"));

			ptr = buttons_[2];
			ptr->caption(STR("Pushed Button"));
			ptr->enable_pushed(true);
		}

		void _m_init_comboxs()
		{
			for(int i = 0; i < 2; ++i)
			{
				auto p = std::make_shared<combox>(*this);
				comboxs_.push_back(p);
				place_.field("comboxs")<<*p; //place_.room(*p, 3, 1);
				p->push_back(STR("Item 0"));
				p->push_back(STR("Item 1"));
			}

			msgbox mb(*this, STR("Item Selected"));
			mb.icon(mb.icon_information);

			auto ptr = comboxs_[0];
			ptr->editable(true);
			ptr->caption(STR("This is an editable combox"));
			ptr->events().selected( [this, mb](const nana::arg_combox& acmb) mutable
			{
				mb<<STR("The item ")<<acmb.widget.option()<<STR(" is selected in editable combox");
				mb();
				//Clear the buffer, otherwise the mb shows the text generated in
				//the last selected event.
				mb.clear();
			});

			ptr = comboxs_[1];
			ptr->caption(STR("This is an uneditable combox"));
			ptr->events().selected ( [this, mb](const nana::arg_combox& acmb) mutable
			{
				mb<<STR("The item ")<<acmb.widget.option()<<STR(" is selected in uneditable combox");
				mb();
				//Clear the buffer, otherwise the mb shows the text generated in
				//the last selected event.
				mb.clear();
			});
		}

		void _m_init_labels()
		{
			for(int i = 0; i < 2; ++i)
			{
				auto p = std::make_shared<label>(*this);
				labels_.push_back(p);
				place_.field("labels")<<*p; //place_.room(, 3, 1);
			}

			auto wd = labels_[0];
			wd->caption(STR("This is a normal label"));

			wd = labels_[1];
			wd->format(true);
			wd->caption(STR("This is a <bold, color=0xFF0000, font=\"Consolas\">formatted label</>"));
		}

		void _m_init_progresses()
		{
			const nana::string tipstr[] = {STR("Unknwon in progress"), STR("Known in progress")};
			for(int i = 0; i < 2; ++i)
			{
				auto p = std::make_shared<progress>(*this);
				place_.field("progresses")<<*p; //place_.room(, 3, 1);
				progresses_.push_back(p);
				p->unknown(i == 0);	//The first progress is unknown mode, the second is known mode.
				tooltip_.set(*p, tipstr[i]);
			}

			timer_.elapse([this](const nana::arg_elapse& a)
			{
				for(auto & p : this->progresses_)
				{
					//Resets the known mode progress if its value reaches the amount value.
					if(false == p->unknown())
					{
						if(p->value() == p->amount())
							p->value(0);
					}
					p->inc();
				}
			});

			timer_.interval(80);
			timer_.start();
			

		}

		void _m_init_tabbar()
		{
			tabbar_.create(*this);
			place_.field("tab")<<tabbar_;

			tabbar_.push_back(STR("listbox"));
			tabpages_.push_back(std::make_shared<tab_page_listbox>(*this));
			tabbar_.push_back(STR("treebox"));
			tabpages_.push_back(std::make_shared<tab_page_treebox>(*this));
			tabbar_.push_back(STR("date_chooser"));
			tabpages_.push_back(std::make_shared<tab_page_datechooser>(*this));
			tabbar_.push_back(STR("radio_group"));
			tabpages_.push_back(std::make_shared<tab_page_radiogroup>(*this));

			std::size_t index = 0;
			for(auto & i : tabpages_)
			{
				tabbar_.relate(index++, *i);
				place_.field("tab_frame").fasten(*i);	//Fasten the tab pages
			}
		}
	private:
		//A layout management
		place place_;
		nana::timer timer_;
		nana::tooltip tooltip_;
		std::vector<std::shared_ptr<button>> buttons_;
		std::vector<std::shared_ptr<combox>> comboxs_;
		std::vector<std::shared_ptr<label>>	labels_;
		std::vector<std::shared_ptr<progress>> progresses_;
		tabbar<nana::string> tabbar_;
		std::vector<std::shared_ptr<panel<false>>> tabpages_;
		
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
