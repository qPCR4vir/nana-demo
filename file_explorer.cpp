/**
*  @file nana-demo\file_explorer.cpp
*  @brief a super widget treelistpathview to browse/explore a tree and the items in each node,
*  with the example of a file system
*
*  The design of a general tree-list explorer widget is not simple because of the many different possibilities 
*  of relation of the "original" data and the data directly in the GUI tree items (nodes), list items, etc.  (where it live primarly) 
*  There are two extreme situations:
*  1- The application is capable of functioning completely independently of the GUI (for example using a CLI) 
*     and maintains a complete functional copy of the data, inclusive the tree structure and the items. 
*     For example: a bioinformatic tool have a tree of organized DNA sequences that need to be in memory 
*     to be compared all vs all in many rounds of partial comparisons.
*  2- The application only need the result of some selection or command from the user and don't need 
*     to know about all the options presented to the user. 
*     For example, it needs only the path to the file and not the entire directory tree from where the user picks the file.
*
*  In the first case the GUI can conserve a reference (any value) to a data node in each tree item showed 
*  and a function to extract a title from it and another function to generate the children nodes. 
*  When refreshing the list corresponding to a tree node it directly uses that reference and a third function 
*  to create the list items and then generate the text in the list columns using again a resolver.
*  In other word, there are situations when one navigate the data using the GUI (and only access the data when required)
*  or when one uses some internal data that the GUI constantly access.
* 
*  In some cases, the GUI can conserve only a text, generates a path for each tree item and from there
*  generate the children and the list items. Or it can conserve a pointer to the data directly.
*
*  Unfortunately most case will be intermediary situations. We will try to implement in parallel the general
*  treelistpathview widget and an example of use of it for file explorer. We will use as original data directly
*  the OS, and the GUI will have only an small part of the data.
*
*/

// http://cpprocks.com/introduction-to-tr2-filesystem-library-in-vs2012/
// https://msdn.microsoft.com/en-us/library/hh874694%28v=vs.140%29.aspx      ---  C++ 14, the <filesystem> header VS2015
// https://msdn.microsoft.com/en-us/library/hh874694%28v=vs.120%29.aspx     --- <filesystem> header VS2013

// test
//#define STD_put_time_NOT_SUPPORTED
//#define NANA_FILESYSTEM_FORCE

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <nana/deploy.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/categorize.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/filesystem/filesystem_ext.hpp>  // #include <nana/filesystem/filesystem_selector.hpp>

namespace fs     = std::filesystem;
namespace fs_ext = nana::filesystem_ext;

//inline directory_only_iterator children(const fs::directory_entry& f) { return directory_only_iterator{ f.path() }; }
//inline fs::directory_iterator  l_items (const fs::directory_entry& f) { return fs::directory_iterator { f.path() }; }
//inline std::string             f_name  (const fs::directory_entry& f) { return f.path().filename().generic_u8string(); }

// the following types could be converted into types parametrs for a generic treelistpathview explorer
using d_node            = fs::directory_entry ;
using d_item            = fs::directory_entry ;
using ct_n_children     = fs_ext::directory_only_iterator;
using ct_l_items        = fs::directory_iterator ;

auto children = [](const d_node& f)->ct_n_children//& 
				{ 
					// Boost can throw an exception "Access is denied"
					// when accessing some system paths, like "C:\Config.Msi"
					try {
						return ct_n_children{ f.path() };
					} catch (...) {
						return ct_n_children();
					}
				};   // ct_n_children& f1(const d_node&);
auto l_items  = [](const d_node& f)->ct_l_items//&    
				{ 
					try {
						return ct_l_items   { f.path() };
					} catch (...) {
						return ct_l_items();
					}
				};   // ct_l_items&  f2(const d_node&);
auto f_name   = [](const d_node& f) 
				{ 
					return  f.path().filename().generic_string();
				};     // std::string  f3(const d_node&);

using f_node_children = decltype (children);
using f_list_items    = decltype (l_items);
using f_node_title    = decltype (f_name);

nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const d_node& item)
{
	ores << f_name(item);
    ores << fs_ext::pretty_file_date(item);//.path()

	if (fs::is_directory(item))
		ores << ("Directory") << "";
	else
	{
		if (item.path().has_extension())
			ores << item.path().extension().generic_string();
//			ores << nana::to_utf8(item.path().extension().generic_wstring());
		else
			ores << ("File");

        ores << fs_ext::pretty_file_size(item);
	}

	return ores;
}

nana::listbox::iresolver& operator>>(nana::listbox::iresolver& ires, d_node& m)
{
	/*std::string  type;
	ires >> m.name >> type >> type;
	m.directory = (type == "Directory");*/
	return ires;
}
//template <class d_node, 
//	      class d_item = d_node::value_type,
//		  class ctn_node_children = std::vector<d_node>& F(d_node&) ,
//		  class ctn_list_items    = std::vector<d_item>& F(d_node&) ,
//		  class ft_node_title = std::string /* title_to_node */(const d_node::value_type& value)
//         >   // some data_node -like template
class explorer :public nana::form
{
	nana::place	           place_ {*this};
	nana::treebox          tree_  {*this};
	nana::listbox	       list_  {*this};
	nana::categorize<d_node> path_  {*this};
	nana::menubar          menu_  {*this};
	nana::panel<false>     view_  {*this},
	                       status_{*this};
	nana::toolbar          tools_ {*this},
	                       nav_   {*this};

	f_node_title           &node_to_title;
	f_node_children	       &node_children;
	f_list_items           &list_items;

	std::string            div_    =
		R"(
				vertical <weight=23 menu>
                         <weight=23 path>
                         <weight=23 tools>
    					 <<tree> |70%   < vertical <list> |50%  <vertical <weight=23 nav>
                                                                          <view>  
                                                                > 
                                         > 
                         >
                         <weight=23 status>  
           )";

public:
	bool      			   force_refresh{false};
	std::string            fake_item;// { "Sorry, this is a fake item!" }; //workaround for VC2013 error C2797: 'explorer::fake_item': list initialization inside member initializer list or non-static data member initializer is not implemented

	using t_node = nana::treebox::item_proxy;
	explorer ( /*d_node& root,*/ 
		       f_node_title     fnt,
		       f_node_children  ctnc,
		       f_list_items     ctni,
		       std::vector<std::pair<std::string, unsigned>> columns,
		       nana::rectangle r= nana::rectangle{ nana::point{50,10}, nana::size{900,600} },
		       std::string title={} )
	:form{r}, node_to_title{fnt}, node_children{ ctnc }, list_items{ ctni }, fake_item("Sorray, this is a fake item")
	{
		place_.div(div_.c_str());
		place_["menu"]  << menu_ ;
		place_["tools"] << tools_ ;
		place_["path"]  << path_ ;
		place_["tree"]  << tree_ ;
		place_["list"]  << list_ ;
		place_["nav"]   << nav_ ;
		place_["view"]  << view_ ;
		place_["status"]<< status_ ;
		place_.collocate();

		//t_node tree_root = add_root(root);
		tree_.events().selected( [this](const nana::arg_treebox &tb_msg)
								 { 
									if (!tb_msg.operated  ) return;

		                            //d_node d{ tree_.make_key_path(tb_msg.item, "/" /*nana::to_utf8(fs::path::preferred_separator)*/ ) 
									//	        + "/" /*d_node::separator*/ };
									
									tree_.auto_draw(false);
									list_.auto_draw(false);
									//path_.auto_draw(false);

									refresh_tnode(tb_msg.item, 	force_refresh);
									refresh_list(tb_msg.item);
									refresh_path(tb_msg.item);
	
									tree_.auto_draw(true);
									list_.auto_draw(true);
									//path_.auto_draw(true);
								 });

		tree_.events().expanded([this](const nana::arg_treebox &tb_msg)
								{
									tree_.auto_draw(false);
									refresh_tnode(tb_msg.item, force_refresh);
									tree_.auto_draw(true);
								});

		//tree_root.select(true)  ;
		menu_.push_back("File");
		menu_.push_back("Help");

		for (auto & col : columns)
			list_.append_header(col.first, col.second);
	}
#if 0 //deprecated
	t_node add_root(d_node & root)
	{
		auto r= tree_.insert(node_to_title(root), node_to_title(root));
		r.value(root);
		signal_child(r);
		return r;
	}

	t_node add_root(std::string & root)
	{
		auto r = tree_.insert(root, root);
		r.value(d_node(root));
		signal_child(r);
		return r;
	}
#endif
	t_node add_root(std::string k, std::string t, d_node && root)   // revise ref types
	{
		auto r = tree_.insert("root_" + k, t); //From 1.7.1, a key should not be a separator \ or /
		r.value(root);
		signal_child(r);
		return r;
	}

	void signal_child(t_node& node)
	{
		if (!force_refresh && !node.child().empty()) return;
		clear_children(node);
		auto& data = node.value<d_node>();
		const ct_n_children& d_c = node_children(data);
		const auto& child_1 = begin(d_c);
		if (child_1 != end(d_c))
			tree_.insert(node, fake_item, "");
	}

	void clear_children(t_node& sel_node)
	{
		while (!sel_node.child().empty())
			tree_.erase(sel_node.child());
	}

	void  refresh_list(t_node& sel_node)
	{
		list_.clear();
		const ct_l_items& items = list_items(sel_node.value<d_node>());
		for (auto &i : items) 
			list_.at(0).append(i, true);
//			list_.at(0).append(nana::to_utf8(i->path().filename().generic_wstring()), true);
	}

	void  refresh_path(t_node& sel_node) {};
	void  refresh_tnode(t_node& sel_node, bool force)
	{
		if (!force && !sel_node.child().empty() && sel_node.child().key()!=fake_item ) return;
		clear_children(sel_node);
		const ct_n_children& d_c = node_children(sel_node.value<d_node>());
		for (auto &n : d_c) 
		{
			auto name = node_to_title(n);
			auto tn = tree_.insert(sel_node, name, name);
			if (tn.empty()) continue;
			tn.value(n);
			signal_child(tn);
		}
	};

};



//using file_explorer_t = explorer< fs::directory_entry,   ///< d_node
//	                              fs::directory_entry,   ///< d_item
//								  decltype (children),	 ///< ctn_node_children
//	                              decltype (l_items), 	 ///< ctn_list_items
//	                              decltype (f_name)	     ///< ft_node_title
//                                >;


// The model / data known how to build a data_node from the path or from the value in the d_node atached to the t_node.
// The tree know how to build the path, and how to build a t_node from d_node


//using namespace nana;

using dir_it = fs_ext::directory_only_iterator;

template <class V,
class C_I, ///< any oder container of value_type with begin, end, != and ++()
class C_N  ///< any oder container of data_node  with begin, end, != and ++() 
>
struct data_node
{
	using value_type = V;
	using cont_it_t = C_I;
	using cont_nd_t = C_I;

	value_type   value;
	C_I          items;   // any oder container of value_type with begin, end, != and ++() 
	C_N          nodes;   // any oder container of data_node  with begin, end, != and ++() 
	std::string  separator;

	data_node(std::string path, std::string  separator = "/")
		: value{ path },
		separator(separator)/*,
							items{ path },
							nodes{ path }*/ {}
};


// "globals functions"
std::string            key(/* value_type*/) { return{}; };
std::string            title(/* value_type*/) { return{}; };


struct dir_node
{
	using value_type = fs::directory_entry;
	using cont_it_t = fs::directory_iterator;
	using cont_nd_t = fs_ext::directory_only_iterator;

	value_type     value;
	cont_it_t      items;
	cont_nd_t      nodes;
	static const std::string separator;

	dir_node(fs::path path)
		: value{ path }, items{ path }, nodes { path }
	{}
		//separator{ "/"  },     /*  std::experimental::filesystem::path::preferred_separator  */


};
const std::string dir_node::separator{ "/" };

//struct d_node : data_node<std::experimental::filesystem::directory_entry, 
//	                      std::experimental::filesystem::directory_entry,
//	                      directory_only_iterator> 
//{
//	using B_T = data_node<std::experimental::filesystem::directory_entry,
//		                  std::experimental::filesystem::directory_entry,
//		                  directory_only_iterator>
//
//	dir_node(std::experimental::filesystem::path path)
//		: B_T{ path }
//	{ 
//		dir{ path },
//		nodes{ path } {}
//};


std::string            key(const dir_node& dn) { return dn.value.path().filename().generic_string(); };
std::string            title(const dir_node& dn) { return key(dn); };


std::string            key(const dir_it& d) { return d->path().filename().generic_string(); };
std::string            title(const dir_it& d) { return key(d); };


//template <class it>
//struct data_it
//{
//	std::vector<it> items;
//	std::vector<data_node> nodes;
//};
//template <class value>
//using title_node = std::string /* title_to_node */ (const value& node_value);


//class file_explorer 	: public nana::form
//{
//	   public:	using  item_proxy = nana::treebox::item_proxy ;
//	            enum class kind {none, filesystem};	    
//
// private:
//
//	nana::place	        place_  {*this};
//
//    nana::button	    btn_new_folder_ {*this, "&New Folder" };
//    nana::categorize<int> path_ {*this};
//	nana::treebox       tree_   {*this};
//	nana::listbox	    ls_file_{*this};
//
//	nana::textbox		filter_ {*this};
//	nana::combox	    cb_types_ {*this};
//
//	nana::label         lb_file_  {*this, "File:" };
//	nana::textbox	    tb_file_  {*this};
//
//	//nana::button        btn_ok_,    btn_cancel_;
//    //bool                io_read_;
//	//nana::string        def_ext_;
//public:
//	struct item_fs
//	{
//		std::string  name;
//		tm            modified_time;
//		bool          directory;
//		uintmax_t     bytes;       // nana::long_long_t
//
//		friend nana::listbox::iresolver& operator>>(nana::listbox::iresolver& ires, item_fs& m)
//		{
//			std::string  type;
//			ires >> m.name >> type >> type ;
//			m.directory = (type == "Directory");
//			return ires;
//		}
//
//		friend nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const item_fs& item)
//		{
//			std::stringstream tm;
//			tm << (item.modified_time.tm_year + 1900) << '-' ;
//			_m_add(tm, item.modified_time.tm_mon + 1) << '-' ;
//			_m_add(tm, item.modified_time.tm_mday )   << ' ' ;
//
//			_m_add(tm, item.modified_time.tm_hour )   << ':' ;
//			_m_add(tm, item.modified_time.tm_min  )   << ':' ;
//			_m_add(tm, item.modified_time.tm_sec  )   ;
//
//			ores<<item.name<<tm.str();
//			if(!item.directory)
//			{
//				auto pos = item.name.find_last_of(('.'));
//				if(pos != item.name.npos && (pos + 1 < item.name.size()))
//					ores<<item.name.substr(pos + 1);
//				else
//					ores<<("File");
//
//				ores<<_m_trans(item.bytes);
//			}
//			else
//				ores<<("Directory");
//			return ores;
//		}
//
//	private:
//		static std::stringstream& _m_add(std::stringstream& ss, unsigned v)
//		{
//			if(v < 10)
//				ss<<'0';
//			ss<<v;
//			return ss;
//		}
//
//		static std::string _m_trans(std::size_t bytes)
//		{
//			const char * ustr[] = {" KB", " MB", " GB", " TB"};
//			std::stringstream ss;
//			if(bytes >= 1024)
//			{
//				double cap = bytes / 1024.0;
//				std::size_t uid = 0;
//				while((cap >= 1024.0) && (uid < sizeof(ustr)/sizeof(char *)))
//				{
//					cap /= 1024.0;
//					++uid;
//				}
//				ss<<cap;
//				auto s = ss.str();
//				auto pos = s.find('.');
//				if(pos != s.npos)
//				{
//					if(pos + 2 < s.size())
//						s.erase(pos + 2);
//				}
//				return s + ustr[uid];
//			}
//			ss<<bytes<<" Bytes";
//			return ss.str();
//		}
//	};
//	
//    std::vector<item_fs> file_container_;
//	struct path_tag
//	{
//		std::string filesystem;
//	}     addr_;
//
//	struct selection_tag
//	{
//		kind type;
//		std::string target;
//	}    selection_;
//	
//	struct tree_node_tag
//	{
//		item_proxy home;
//		item_proxy filesystem;
//	}nodes_;
//
//	static std::string saved_init_path;
//	static std::string saved_selected_path;
//
//
//	struct pred_sort_fs
//	{
//		bool operator()(const item_fs& a, const item_fs& b) const
//		{
//			return ((a.directory != b.directory) && a.directory);
//		}
//	};
//public:
//
//	file_explorer( const std::string& title= "Nana File Explorer") 	: form( nana::API::make_center( 630, 440) )
//	{
//		path_.splitstr("/");
//		path_.events().selected.connect_unignorable([this](const nana::arg_categorize<int>&)
//		{
//			auto path = path_.caption();
//			auto root = path.substr(0, path.find(('/')));
//			if(root ==  ("HOME"))
//				path.replace(0, 4, def_rootname);//nana::experimental::filesystem::path_user());   //  REPLACE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! to filesystem_ext.hhp
//			else if(root == ("FILESYSTEM"))
//				path.erase(0, 10);
//			else
//				throw std::runtime_error("Nana.GUI.Filebox: Wrong categorize path");
//
//			if(path.size() == 0) path = ("/");
//			_m_load_cat_path(path);
//		});
//
//		btn_new_folder_.events().click.connect_unignorable([this](const nana::arg_click&)
//		{
//              // use inputbox
//			nana::form fm(this->handle(), nana::API::make_center(*this, 300, 35));
//			fm.caption(("Name the new folder"));
//
//			nana::textbox folder(fm, nana::rectangle(5, 5, 160, 25));
//			folder.multi_lines(false);
//
//			nana::button btn(fm, nana::rectangle(170, 5, 60, 25));
//			btn.caption(("Create"));
//
//			btn.events().click.connect_unignorable(folder_creator(*this, fm, folder));
//
//			nana::button btn_cancel(fm, nana::rectangle(235, 5, 60, 25));
//			btn_cancel.caption(("Cancel"));
//
//			btn_cancel.events().click.connect_unignorable([&fm](const nana::arg_click&)
//			{
//				fm.close();
//			});
//			nana::API::modal_window(fm);
//		});
//
//		ls_file_.append_header(("Name"    ), 190);
//		ls_file_.append_header(("Modified"), 145);
//		ls_file_.append_header(("Type"    ),  80);
//		ls_file_.append_header(("Size"    ),  70);
//
//		auto fn_sel_file = [this](const nana::arg_mouse& arg){ 	_m_sel_file(arg);		};
//
//		ls_file_.events().dbl_click .connect_unignorable(fn_sel_file);
//		ls_file_.events().mouse_down.connect_unignorable(fn_sel_file);
//
//		ls_file_.set_sort_compare(0, [](const std::string& a, nana::any* fs_a,
//                                        const std::string& b, nana::any* fs_b, bool reverse) -> bool
//			{
//				int dira = nana::any_cast<item_fs>(fs_a)->directory ? 1 : 0;
//				int dirb = nana::any_cast<item_fs>(fs_b)->directory ? 1 : 0;
//				if(dira != dirb)
//					return (reverse ? dira < dirb : dira > dirb);
//
//				std::size_t seek_a = 0;
//				std::size_t seek_b = 0;
//
//				while(true)
//				{
//					std::size_t pos_a = a.find_first_of(("0123456789"), seek_a);
//					std::size_t pos_b = b.find_first_of(("0123456789"), seek_b);
//
//					if((pos_a != a.npos) && (pos_a == pos_b))
//					{
//						nana::cistring text_a = a.substr(seek_a, pos_a - seek_a).data();
//						nana::cistring text_b = b.substr(seek_b, pos_b - seek_b).data();
//
//						if(text_a != text_b)
//							return (reverse ? text_a > text_b : text_a < text_b);
//
//						std::size_t end_a = a.find_first_not_of(("0123456789"), pos_a + 1);
//						std::size_t end_b = b.find_first_not_of(("0123456789"), pos_b + 1);
//			
//						std::string num_a = a.substr(pos_a, end_a != a.npos ? end_a - pos_a : a.npos);
//						std::string num_b = b.substr(pos_b, end_b != b.npos ? end_b - pos_b : b.npos);
//			
//						if(num_a != num_b)
//						{
//							double ai = std::stod(num_a, 0);
//							double bi = std::stod(num_b, 0);
//							if(ai != bi)
//								return (reverse ? ai > bi : ai < bi);
//						}
//
//						seek_a = end_a;
//						seek_b = end_b;
//					}
//					else
//						break;
//				}
//				if(seek_a == a.npos)
//					seek_a = 0;
//				if(seek_b == b.npos)
//					seek_b = 0;
//
//				nana::cistring cia = a.data();
//				nana::cistring cib = b.data();
//				if(seek_a == seek_b && seek_a == 0)
//					return (reverse ? cia > cib : cia < cib);
//				return (reverse ? cia.substr(seek_a) > cib.substr(seek_b) : cia.substr(seek_a) < cib.substr(seek_b));
//			});
//		ls_file_.set_sort_compare(2, [](const std::string& a, nana::any* anyptr_a,
//                                        const std::string& b, nana::any* anyptr_b, bool reverse) -> bool
//			{
//				int dir1 =  nana::any_cast<item_fs>(anyptr_a)->directory ? 1 : 0;
//				int dir2 =  nana::any_cast<item_fs>(anyptr_b)->directory ? 1 : 0;
//				if(dir1 != dir2)
//					return (reverse ? dir1 < dir2 : dir1 > dir2);
//
//				return (reverse ? a > b : a < b);
//			});
//		ls_file_.set_sort_compare(3, [this](const std::string&, nana::any* anyptr_a,
//                                            const std::string&, nana::any* anyptr_b, bool reverse) -> bool
//			{
//				item_fs * fsa = nana::any_cast  <item_fs>(anyptr_a);
//				item_fs * fsb = nana::any_cast  <item_fs>(anyptr_b);
//				return (reverse ? fsa->bytes > fsb->bytes : fsa->bytes < fsb->bytes);
//			});
//		tb_file_.multi_lines(false);
//
//		tb_file_.events().key_char.connect_unignorable([this](const nana::arg_keyboard& arg)
//		{
//			if(arg.key == nana::keyboard::enter)
//				_m_ok();
//		});
//
//		cb_types_.editable(false);
//		cb_types_.events().selected.connect_unignorable([this](const nana::arg_combox&){ _m_list_fs(); });
//
//		selection_.type = kind::none;
//		_m_layout();
//		_m_init_tree();
//	}
//
//	void load_fs(const std::string& init_path, const std::string& init_file)
//	{
//		//Simulate the behavior like Windows7's 
//        //lpstrInitialDir(http://msdn.microsoft.com/en-us/library/windows/desktop/ms646839%28v=vs.85%29.aspx)
//
//		//Phase 1
//		std::string dir;
//
//		auto pos = init_file.find_last_of( ("\\/"));
//		std::string file_with_path_removed = (pos != init_file.npos ? init_file.substr(pos + 1) : init_file);
//
//		if(saved_init_path != init_path)
//		{
//			if(saved_init_path.size() == 0)
//				saved_init_path = init_path;
//
//			//Phase 2: Check whether init_file contains a path
//			if(file_with_path_removed == init_file)
//			{
//				//Phase 3: Check whether init_path is empty
//				if(init_path.size())
//					dir = init_path;
//			}
//			else
//				dir = init_file.substr(0, pos);
//		}
//		else
//			dir = saved_selected_path;
//
//		_m_load_cat_path(dir.size() ? dir : def_rootname); //nana::experimental::filesystem::path_user());
//
//		tb_file_.caption(file_with_path_removed);					
//	}
//		
//	void add_filter(const std::string& desc, const std::string& type)
//	{
//		std::size_t i = cb_types_.the_number_of_options();
//		cb_types_.push_back(desc);
//		if(0 == i)
//			cb_types_.option(0);
//
//		std::vector<std::string> v;
//		std::size_t beg = 0;
//		while(true)
//		{
//			auto pos = type.find( (';'), beg);
//			auto ext = type.substr(beg, pos == type.npos ? type.npos : pos - beg);
//			auto dot = ext.find( ('.'));
//			if((dot != ext.npos) && (dot + 1 < ext.size()))
//			{
//				ext.erase(0, dot + 1);
//				if(ext ==  ("*"))
//				{
//					v.clear();
//					break;
//				}
//				else
//					v.push_back(ext);
//			}
//			if(pos == type.npos)
//				break;
//			beg = pos + 1;
//		}
//		if(v.size())
//			cb_types_.anyobj(i, v);
//	}
//
//	bool file(std::string& fs) const
//	{
//		if(selection_.type == kind::none)
//			return false;
//
//		auto pos = selection_.target.find_last_of( ("\\/"));
//		if(pos != selection_.target.npos)
//			saved_selected_path = selection_.target.substr(0, pos);
//		else
//			saved_selected_path.clear();
//
//		fs = selection_.target;
//		return true;
//	}
//private:
//	void _m_layout()
//	{	
//		place_.div(	"vert"
//				"<weight=34 margin=5          path       arrange=[variable,200] gap=5 >"
//				"<weight=30 margin=[0,0,5,10] new_folder arrange=[100]                >"
//				"<                            content    arrange=[180]          gap=[5]>"
//                "<weight=8>"
//				"<weight=26 <weight=100> "
//                "           <weight=60 vert   label      margin=[0,0,0,5]>"
//				"           <                 file       margin=[0,18,0,5] arrange=[variable,variable,190] gap=[10]>>"
//				"<weight=48 margin=[8,0,14]<>" );
//
//		place_.field( "path" )      << path_     /*<<filter_*/;
//		place_.field( "new_folder" )<< btn_new_folder_ ;
//		place_.field( "content" )   << tree_  <<  ls_file_;
//		place_.field( "label" )     << lb_file_;
//		place_.field( "file" )      << tb_file_ << cb_types_;
//		place_.collocate();
//	}
//
//	void _m_init_tree()
//	{
//		using namespace nana::experimental::filesystem;
//
//		//The path in linux start with the character '/', the root key should be
//		//"FS.HOME", "FS.ROOT". Because a key of the tree widget should not be '/'
//		auto node_home = tree_.insert( ("FS.HOME"),  ("Home"));
//		node_home.value(kind::filesystem);
//		auto node_filesystem = tree_.insert( ("FS.ROOT"),  ("Filesystem"));
//		node_filesystem.value(kind::filesystem);
//		using SubDirectories =  directory_iterator;
//
//		for (const auto& dir : SubDirectories{ path_user() })
//		{
//			if ( !is_directory(dir) || (!dir.path().filename().empty() && dir.path().filename().generic_u8string() [0] == '.')) continue;
//
//			item_proxy node = tree_.insert(node_home, dir.path().filename().generic_u8string(), dir.path().filename().generic_u8string());
//			if( ! node.empty() )
//			{
//				node.value(dir); //node.value(kind::filesystem);
//				break;
//			}
//		}
//
//		for (const auto& dir : SubDirectories{  ("/") })
//		{
//			if ( !is_directory(dir) || (!dir.path().filename().empty() && dir.path().filename().generic_u8string()[0] == '.')) continue;
//
//			item_proxy node = tree_.insert(node_home, dir.path().filename().generic_u8string(), dir.path().filename().generic_u8string());
//			if( ! node.empty() )
//			{
//				node.value(dir); //node.value(kind::filesystem);
//				break;
//			}
//		}
//
//		tree_.events().expanded.connect_unignorable([this](const nana::arg_treebox& arg)
//		{
//			_m_tr_expand(arg.item, arg.operated);
//		});
//
//		tree_.events().selected.connect_unignorable([this](const nana::arg_treebox& arg)
//		{
//			if(arg.operated &&  ! arg.item.empty() )
//			{
//				auto path = tree_.make_key_path(arg.item,  ("/")) +  ("/");
//				_m_resolute_path(path);
//				_m_load_cat_path(path);
//			}
//		});
//	}
//
//	std::string _m_resolute_path(std::string& path)
//	{
//		auto pos = path.find( ('/'));
//		if(pos != path.npos)
//		{
//			auto begstr = path.substr(0, pos);
//			if(begstr ==  ("FS.HOME"))
//				path.replace(0, 7, def_rootname);// nana::experimental::filesystem::path_user());
//			else
//				path.erase(0, pos);
//			return begstr;
//		}
//		return {};
//	}
//
//	void _m_load_path(const std::string& path)
//	{
//		addr_.filesystem = path;
//		if(addr_.filesystem.size() && addr_.filesystem[addr_.filesystem.size() - 1] !=  ('/'))
//			addr_.filesystem +=  ('/');
//
//		file_container_.clear();
//
//		using namespace std::experimental::filesystem;
//
//		file_status  fattr ; attribute ;
//		directory_iterator end;
//		for(directory_iterator i(path); i != end; ++i)
//		{
//			if((!i->path().filename().empty() == 0) || (i->path().filename().generic_u8string()[0] ==  ('.')))
//				continue;
//			item_fs m;
//			m.name = i->path().filename();
//			if(file_attrib(path + m.name, fattr))
//			{
//				m.bytes = fattr.size;
//				m.directory = fattr.directory;
//				m.modified_time = fattr.modified;
//			}
//			else
//			{
//				m.bytes = 0;
//				m.directory = i->attr.directory;
//				modified_file_time(path + i->path().filename().generic_u8string() , m.modified_time);
//			}
//
//			file_container_.push_back(m);
//
//			if(m.directory)
//				path_.childset(m.name, 0);
//		}
//		std::sort(file_container_.begin(), file_container_.end(), pred_sort_fs());
//	}
//
//	void _m_load_cat_path(std::string path)
//	{
//		if((path.size() == 0) || (path[path.size() - 1] !=  ('/')))
//			path +=  ('/');
//
//		auto beg_node = tree_.selected();
//		while(!beg_node.empty() && (beg_node != nodes_.home) && (beg_node != nodes_.filesystem))
//			beg_node = beg_node.owner();
//			
//		auto head = filesystem::path_user();
//		if(path.size() >= head.size() && (path.substr(0, head.size()) == head))
//		{//This is HOME
//			path_.caption( ("HOME"));
//			if(beg_node != nodes_.home)
//				nodes_.home->select(true);
//		}
//		else
//		{	//Redirect to '/'
//			path_.caption( ("FILESYSTEM"));
//			if(beg_node != nodes_.filesystem)
//				nodes_.filesystem->select(true);
//			head.clear();
//		}
//
//		if(head.size() == 0 || head[head.size() - 1] !=  ('/'))
//			head +=  ('/');
//
//		using namespace nana::experimental::filesystem;
//		directory_iterator end;
//		for(directory_iterator i(head); i != end; ++i)
//		{
//			if(i->attr.directory)
//				path_.childset(i->path().filename().generic_u8string(), 0);
//		}
//		auto cat_path = path_.caption();
//		if(cat_path.size() && cat_path[cat_path.size() - 1] !=  ('/'))
//			cat_path +=  ('/');
//
//		auto beg = head.size();
//		while(true)
//		{
//			auto pos = path.find( ('/'), beg);
//			auto folder = path.substr(beg, pos != path.npos ? pos - beg: path.npos);
//			if(folder.size() == 0)  ;
//			(cat_path += folder) +=  ('/');
//			(head += folder) +=  ('/');
//			path_.caption(cat_path);
//				
//			for(nana::experimental::filesystem::directory_iterator i(head); i != end; ++i)
//			{
//				if (i->attr.directory)
//					path_.childset(i->path().filename().generic_u8string(), 0);
//			}
//
//			if(pos == path.npos)
//				break;
//			beg = pos + 1;
//		}
//		_m_load_path(path);
//		_m_list_fs();
//	}
//
//	bool _m_filter_allowed(const std::string& name, bool is_dir, const std::string& filter, const std::vector<std::string>* extension) const
//	{
//		if(filter.size() && (name.find(filter) == filter.npos))
//			return false;
//
//		if((is_dir || 0 == extension) || (0 == extension->size())) return true;
//
//		for(auto & extstr : *extension)
//		{
//			auto pos = name.rfind(extstr);
//			if((pos != name.npos) && (name.size() == pos + extstr.size()))
//				return true;
//		}
//		return false;
//	}
//
//	void _m_list_fs()
//	{
//		std::string filter = filter_.caption();
//		ls_file_.auto_draw(false);
//
//		ls_file_.clear();
//
//		std::vector<std::string>* ext_types = cb_types_.anyobj<std::vector<std::string> >(cb_types_.option());
//		auto cat = ls_file_.at(0);
//		for(auto & fs: file_container_)
//		{
//			if(_m_filter_allowed(fs.name, fs.directory, filter, ext_types))
//			{
//				cat.append(fs).value(fs);
//			}
//		}
//		ls_file_.auto_draw(true);
//	}
//
//	void _m_finish(kind type, const std::string& tar)
//	{
//		selection_.target = tar;
//		selection_.type = type;
//		close();
//	}
//
//	struct folder_creator
//	{
//		folder_creator(file_explorer& fb, nana::form & fm, nana::textbox& tx)
//			:	fb_(fb), fm_(fm), tx_path_(tx)
//		{}
//
//		void operator()()
//		{
//			auto path = tx_path_.caption();
//
//			nana::msgbox mb(fm_,  ("Create Folder"));
//			mb.icon(nana::msgbox::icon_warning);
//			if(0 == path.size() || path[0] ==  ('.') || path[0] ==  ('/'))
//			{
//				mb<< ("Please input a valid name for the new folder.");
//				mb();
//				return;
//			}
//				
//			bool if_exist;
//			if(false == filesystem::mkdir(fb_.addr_.filesystem + path, if_exist))
//			{
//				if(if_exist)
//					mb<< ("The folder is existing, please rename it.");
//				else
//					mb<< ("Failed to create the folder, please rename it.");
//				mb();
//				return;
//			}
//			fb_._m_load_cat_path(fb_.addr_.filesystem);
//			fm_.close();
//		}
//
//		file_explorer& fb_;
//		nana::form& fm_;
//		nana::textbox & tx_path_;
//	};
//
//	bool _m_append_def_extension(std::string& tar) const
//	{
//		auto dotpos = tar.find_last_of( ('.'));
//		if(dotpos != tar.npos)
//		{
//			auto pos = tar.find_last_of( ("/\\"));
//			if(pos == tar.npos || pos < dotpos)
//				return false;
//		}
//
//		std::vector<nana::string> * exts = cb_types_.anyobj<std::vector<std::string> >(cb_types_.option());
//		if(0 == exts || exts->size() == 0)	return false;
//			
//		auto & ext = exts->at(0);
//		tar += ext;
//		return true;
//	}
//private:
//	void _m_sel_file(const nana::arg_mouse& arg)
//	{
//		auto sel = ls_file_.selected();
//		if(sel.empty())
//			return;
//			
//		auto index = sel[0];
//		item_fs m;
//		ls_file_.at(index).resolve_to(m);
//
//		if(nana::event_code::dbl_click == arg.evt_code)
//		{
//			if(m.directory)
//				_m_load_cat_path(addr_.filesystem + m.name +  ("/"));
//			else
//				_m_finish(kind::filesystem, addr_.filesystem + m.name);
//		}
//		else
//		{
//			if(false == m.directory)
//			{
//				selection_.target = addr_.filesystem + m.name;
//				tb_file_.caption(m.name);
//			}
//		}
//	}
//
//	void _m_ok()
//	{
//		if(0 == selection_.target.size())
//		{
//			auto file = tb_file_.caption();
//			if(file.size())
//			{
//				if(file[0] ==  ('.'))
//				{
//					nana::msgbox mb(*this, caption());
//					mb.icon(nana::msgbox::icon_warning);
//					mb<<file<<std::endl<< ("The filename is invalid.");
//					mb();
//					return;
//				}
//				std::string tar;
//				if(file[0] ==  ('/'))
//					tar = file;
//				else
//					tar = addr_.filesystem + file;
//
//				bool good = true;
//				filesystem::attribute attr;
//				if(filesystem::file_attrib(tar, attr) == false)
//				{
//					if(_m_append_def_extension(tar))
//						good = filesystem::file_attrib(tar, attr);
//					else
//						good = false;
//				}
//					
//				if(good && attr.is_directory)
//				{
//					_m_load_cat_path(tar);
//					tb_file_.caption( (""));
//					return;
//				}
//					
//				{
//					if(good)
//					{
//						nana::msgbox mb(*this, caption(), nana::msgbox::yes_no);
//						mb.icon(nana::msgbox::icon_question);
//						mb<< ("The input file is existing, do you want to overwrite it?");
//						if(nana::msgbox::pick_no == mb())
//							return;
//					}
//					_m_finish(kind::filesystem, tar);
//				}
//			}
//		}
//		else
//			_m_finish(kind::filesystem, selection_.target);
//	}
//
//	void _m_tr_expand(item_proxy node, bool exp)
//	{
//		if(false == exp) return;
//
//		if( ! node.empty())
//		{
//			std::string path = tree_.make_key_path(node,  ("/")) +  ("/");
//			_m_resolute_path(path);
//
//			using namespace nana::experimental::filesystem;
//			directory_iterator end;
//			for(directory_iterator i(path); i != end; ++i)
//			{
//				if((false == i->attr.directory) || (i->path().filename().size() && i->path().filename().generic_u8string()[0] == '.')) continue;
//				auto child = node.append(i->path().filename().generic_u8string(), i->path().filename().generic_u8string(), *i);
//				if(!child.empty())
//				{
//					for(directory_iterator u(path + i->path().filename().generic_u8string()); u != end; ++u)
//					{
//						if(! u->attr.directory || (!u->path().filename().empty() && u->path().filename().generic_u8string()[0] == '.')) continue;
//
//						child.append(u->path().filename().generic_u8string(), u->path().filename().generic_u8string(), *i);
//						break;
//					}
//				}
//			}
//		}
//	}
//
//};//end class filebox_implement

//std::string file_explorer::saved_init_path;
//std::string file_explorer::saved_selected_path;


int main()
{
    // using namespace nana;
	// d_node d{ def_rootstr };
	explorer fb( f_name, 
		         children, 
		         l_items,
				 { { ("Name"), 190 }, { ("Modified"), 145 }, { ("Type"), 80 }, { ("Size"), 70 }  });

	fb.add_root(fs_ext::def_root, fs_ext::def_rootname, d_node{ fs_ext::def_rootstr }).select(true);

    fb.show();
    nana::exec();
    //char a;
    //std::cin >> a;
}
