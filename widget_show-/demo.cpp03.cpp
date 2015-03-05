/*
 *	This is a demo of Nana C++ Library
 *	Author: Jinhao
 *	The demo requires Nana 0.6
 *	Screenshot at http://sourceforge.net/projects/stdex
 */
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/categorize.hpp>
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
#include <nana/gui/timer.hpp>
#include <nana/gui/tooltip.hpp>
#include <memory>
#include <vector>

namespace demo
{
	using namespace nana::gui;

	class tab_page_listbox
		: public panel<false>
	{
	public:
		tab_page_listbox(window wd)
			: panel<false>(wd)
		{
			place_.bind(*this);
			place_.div("<list vertical>");

			listbox_.create(*this);
			listbox_.append_header(STR("Supported compilers"), 200);
			nana::gui::listbox::cat_proxy cat = listbox_.append(STR("Nana.C++03"));
			cat.push_back(STR("GCC 3.4 and later"));
			cat.push_back(STR("Visual C++ 2003 and later"));

			cat = listbox_.append(STR("Nana.C++11"));
			cat.push_back(STR("GCC 4.6 and later"));
			cat.push_back(STR("Visual C++ 2012 and later"));

			checkbox_.create(*this);
			checkbox_.caption(STR("Checkable Listbox"));
			checkbox_.make_event<events::click>(nana::make_fun(*this, &tab_page_listbox::_m_checked));

			place_.field("list")<<listbox_<<5<<place_.fixed(checkbox_, 30);
		}
	private:
		void _m_checked()
		{
			this->listbox_.checkable(this->checkbox_.checked());
		}
	private:
		place place_;
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
			treebox_.ext_event().expand = nana::make_fun(*this, &tab_page_treebox::_m_expand);
		}
	private:
		void _m_expand(nana::gui::window, item_proxy node, bool exp)
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

			place_.div("<weight=5><vertical<check vertical><bottom vertical weight=50>><weight=5>");

			const nana::string str[6] = {
					STR("Airbus"), STR("AHTOHOB"),
					STR("Boeing"), STR("Bombardier"),
					STR("Cessna"), STR("EMBRAER")};
					
			for(int i = 0; i < 6; ++i)
			{
				nana::shared_ptr<checkbox> p(new checkbox(*this));
				box_.push_back(p);

				//Add the checkbox to the radio group. The radio group does not
				//manage the life of checkboxs.
				group_.add(*p);
				place_.field("check")<<place_.fixed(*p, 20)<<5;
				p->caption(str[i]);
				p->make_event<events::click>(nana::make_fun(*this, &tab_page_radiogroup::_m_selected));	
			}
			
			label_.create(*this);
			label_.caption(STR("Select an airplane manufacturer"));

			categorize_.create(*this);

			place_.field("bottom")<<place_.fixed(label_, 20)<<5<<place_.fixed(categorize_, 20);

			std::map<nana::string, std::vector<nana::string> > map;
			std::vector<nana::string>* p = &(map[str[0]]);
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

			for(int i = 0; i < 6; ++i)
			{
				categorize_.caption(STR("Manufacturer"));
				categorize_.insert(str[i], 0);
				std::vector<nana::string> & v = map[str[i]];
				for(std::vector<nana::string>::iterator i = v.begin(); i != v.end(); ++i)
					categorize_.childset(*i, 0);
			}
		}
	private:
		void _m_selected()
		{
			std::size_t index = group_.checked();
			nana::string str = box_[index]->caption();

			label_.caption(STR("You have selected ") + str);
			categorize_.caption(STR("Manufacturer\\") + str);		
		}
	private:
		place place_;
		radio_group group_;
		std::vector<nana::shared_ptr<checkbox> > box_;
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
			place_.div("vertical<weight=40%<weight=10><vertical <weight=10><table grid[6,4] gap = 10>>><tab weight=20><tab_frame>");

			_m_init_buttons();
			_m_init_comboxs();
			_m_init_labels();
			_m_init_progresses();
			_m_init_tabbar();

			this->make_event<events::unload>(nana::make_fun(*this, &widget_show::_m_ask_for_quit));
			
			place_.collocate();
		}
	private:
		void _m_ask_for_quit(const eventinfo& ei)
		{
			msgbox mb(this->handle(), STR("Question"), msgbox::yes_no);
			mb.icon(mb.icon_question);
			mb<<STR("Are you sure you want to exit the demo?");
			ei.unload.cancel = (mb.pick_no == mb());
		}
	private:
		void _m_init_buttons()
		{

			msgbox mb(*this, STR("Msgbox"));
			mb.icon(mb.icon_information);
			mb<<STR("Button Clicked");
			for(int i = 0; i < 3; ++i)
			{
				nana::shared_ptr<button> p(new button(*this));
				buttons_.push_back(p);
				place_.field("table")<<place_.room(*p, 2, 1);
				p->make_event<events::click>(mb);
			}
			
			nana::shared_ptr<button> ptr = buttons_[0];
			ptr->caption(STR("Normal Button"));

			ptr = buttons_[1];
			ptr->icon(STR("image.ico"));
			ptr->caption(STR("Button with An Image"));

			ptr = buttons_[2];
			ptr->caption(STR("Pushed Button"));
			ptr->enable_pushed(true);
		}

		void _m_init_comboxs()
		{
			for(int i = 0; i < 2; ++i)
			{
				nana::shared_ptr<combox> p(new combox(*this));
				comboxs_.push_back(p);
				place_.field("table")<<place_.room(*p, 3, 1);
				p->push_back(STR("Item 0"));
				p->push_back(STR("Item 1"));
			}

			nana::shared_ptr<combox> ptr = comboxs_[0];
			ptr->editable(true);
			ptr->caption(STR("This is an editable combox"));
			ptr->ext_event().selected = nana::make_fun(*this, &widget_show::_m_combox_selected);

			ptr = comboxs_[1];
			ptr->caption(STR("This is an uneditable combox"));
			ptr->ext_event().selected = nana::make_fun(*this, &widget_show::_m_combox_selected);
		}

		void _m_combox_selected(combox & cmb)
		{
			msgbox mb(*this, STR("Item Selected"));
			mb.icon(mb.icon_information);
			mb<<STR("The item ")<<cmb.option()<<(cmb.editable() ?
				STR(" is selected in editable combox") : STR(" is selected in uneditable combox"));
			mb();
		}

		void _m_init_labels()
		{
			for(int i = 0; i < 2; ++i)
			{
				nana::shared_ptr<label> p(new label(*this));
				labels_.push_back(p);
				place_.field("table")<<place_.room(*p, 3, 1);
			}

			nana::shared_ptr<label> wd = labels_[0];
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
				nana::shared_ptr<progress> p(new progress(*this));
				place_.field("table")<<place_.room(*p, 3, 1);
				progresses_.push_back(p);
				p->unknown(i == 0);	//The first progress is unknown mode, the second is known mode.
				tooltip_.set(*p, tipstr[i]);
			}

			timer_.make_tick(nana::make_fun(*this, &widget_show::_m_timer_tick));
			timer_.interval(80);
		}

		void _m_timer_tick()
		{
			for(int i = 0; i < 2; ++i)
			{
				nana::shared_ptr<progress> p = progresses_[i];
				if(false == p->unknown())
				{
					if(p->value() == p->amount())
						p->value(0);
				}
				p->inc();				
			}
		}

		void _m_init_tabbar()
		{
			tabbar_.create(*this);
			place_.field("tab")<<tabbar_;

			typedef nana::shared_ptr<panel<false> > panel_shared_t;
			tabbar_.push_back(STR("listbox"));
			tabpages_.push_back(panel_shared_t(new tab_page_listbox(*this)));
			tabbar_.push_back(STR("treebox"));
			tabpages_.push_back(panel_shared_t(new tab_page_treebox(*this)));
			tabbar_.push_back(STR("date_chooser"));
			tabpages_.push_back(panel_shared_t(new tab_page_datechooser(*this)));
			tabbar_.push_back(STR("radio_group"));
			tabpages_.push_back(panel_shared_t(new tab_page_radiogroup(*this)));

			std::size_t index = 0;
			for(std::vector<panel_shared_t>::iterator i = tabpages_.begin(); i != tabpages_.end(); ++i)
			{
				tabbar_.relate(index++, *(i->get()));
				place_.field("tab_frame").fasten(*(i->get()));	//Fasten the tab pages
			}
		}
	private:
		//A gird layout management
		place place_;
		nana::gui::timer timer_;
		nana::gui::tooltip tooltip_;
		std::vector<nana::shared_ptr<button>> buttons_;
		std::vector<nana::shared_ptr<combox>> comboxs_;
		std::vector<nana::shared_ptr<label>>	labels_;
		std::vector<nana::shared_ptr<progress>> progresses_;
		tabbar<nana::string> tabbar_;
		std::vector<nana::shared_ptr<panel<false> > > tabpages_;
		
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
