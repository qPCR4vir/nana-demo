/*
 *	The FreeMe - A sample of Nana C++ Library
 *	Copyright(C) 2007-2009 Jinhao
 *
 *	Distributed under the Nana Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://stdex.sourceforge.net/LICENSE_1_0.txt)
 *
 *	@description:
 *		This sample shows you how to write an application with Nana C++ Library.
 */

// http://nanapro.sourceforge.net/help/tutorials/thefreeme.htm

#ifndef PATH_CREATOR_HPP
#define PATH_CREATOR_HPP
#include <nana/gui/wvl.hpp>
#include <shlobj.h>
#include <windows.h>


namespace path
{
	namespace detail
	{
		nana::string reg_string(HKEY hkey, const nana::char_t* subkey, const nana::char_t* value)
		{
			HKEY key;
			nana::string str;
			if(ERROR_SUCCESS == ::RegOpenKey(hkey, subkey, &key))
			{
				DWORD bufsize = MAX_PATH;
				nana::char_t * buf = new nana::char_t[bufsize];

				LONG result;
				DWORD len = bufsize;;
				while(ERROR_MORE_DATA == (result = ::RegQueryValueEx(key, value, 0, 0, reinterpret_cast<LPBYTE>(buf), &len)))
				{
					delete [] buf;
					bufsize += MAX_PATH;
					len = bufsize;
					buf = new nana::char_t[len];
				}

				if(ERROR_SUCCESS == result)
					str = buf;

				delete [] buf;
			}
			return str;
		}
	}

	struct tasks
	{
		enum{ dllcache, prefetch, user_temporary, windows_temporary, internet_temporary,
			wer_user_archive, wer_user_queue, wer_system_archive, wer_system_queue,
			count};
	};
	
	template<int ID>
	struct creator
	{
		static nana::string fetch();
		static nana::string msg();
	};
	
	template<>
	struct creator<tasks::dllcache>
	{
		static nana::string fetch()
		{
			nana::char_t temp[260];
			::GetSystemDirectory(temp, 260);
			return nana::string(temp) + STR("\\dllcache");
		}
		
		static nana::string msg()
		{
			return STR("DLLCache");	
		}
	};

	template<>
	struct creator<tasks::prefetch>
	{
		static nana::string fetch()
		{
			nana::char_t temp[260];
			::GetWindowsDirectory(temp, 260);
			return nana::string(temp) + STR("\\Prefetch");
		}
		
		static nana::string msg()
		{
			return STR("Prefetch");	
		}
	};

	template<>
	struct creator<tasks::user_temporary>
	{
		static nana::string fetch()
		{
			nana::char_t temp[260];
			::GetTempPath(260, temp);
			return nana::string(temp).substr(0, nana::strlen(temp) - 1);
		}
		
		static nana::string msg()
		{
			return STR("Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::windows_temporary>
	{
		static nana::string fetch()
		{
			nana::char_t temp[260];
			::GetWindowsDirectory(temp, 260);
			return nana::string(temp) + STR("\\Temp");
		}
		
		static nana::string msg()
		{
			return STR("Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::internet_temporary>
	{
		static nana::string fetch()
		{
			nana::char_t temp[MAX_PATH];
			::SHGetFolderPath(0, CSIDL_INTERNET_CACHE, 0, 0, temp);
			return nana::string(temp);
		}
		
		static nana::string msg()
		{
			return STR("Internet Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::wer_user_archive>
	{
		static nana::string fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Archive Files"), STR("Folder"));
		}

		static nana::string msg()
		{
			return STR("Windows Error Reporting Archive Files");
		}
	};

	template<>
	struct creator<tasks::wer_user_queue>
	{
		static nana::string fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Queue Files"), STR("Folder"));
		}

		static nana::string msg()
		{
			return STR("Windows Error Reporting Queue Files");
		}
	};

	template<>
	struct creator<tasks::wer_system_archive>
	{
		static nana::string fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Archive Files"), STR("Folder"));
		}

		static nana::string msg()
		{
			return STR("Windows Error Reporting System Archive Files");
		}
	};

	template<>
	struct creator<tasks::wer_system_queue>
	{
		static nana::string fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Queue Files"), STR("Folder"));
		}

		static nana::string msg()
		{
			return STR("Windows Error Reporting System Queue Files");
		}
	};
}

#endif

/*
 *	The FreeMe - A sample of Nana C++ Library
 *	Copyright(C) 2007-2009 Jinhao
 *
 *	Distributed under the Nana Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://stdex.sourceforge.net/LICENSE_1_0.txt)
 *
 *	@description:
 *		This sample shows you how to write an application with Nana C++ Library.
 */
// TODO: , HACK, or UNDONE
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/filesystem/file_iterator.hpp>
#include <nana/filesystem/fs_utility.hpp>
//#include <nana/threads/thread.hpp>
#include <vector>
#include <deque>
//#include "path_creator.hpp"
	
/*
 *	class messenger
 *	@brief:	messenger is a class that used for delivering message from junk_sweeper
 *			to other classes.
 */
class messenger
{
public:
	virtual ~messenger(){}
	virtual void active(){}
	virtual void message(const nana::string&){}
};

/*
 *	class junk_sweeper
 *	@brief: search temporary files and clear these files.
 */
class junk_sweeper
{
	typedef junk_sweeper self_type;
public:
	typedef nana::filesystem::file_iterator file_iterator;
	typedef file_iterator::value_type file_info_type;

	junk_sweeper()
		:bytes_(0), size_(0), ready_(false)
	{}

	void search(messenger& m)
	{
		size_ = 0;
		bytes_ = 0;
		ready_ = false;
		std::vector<std::deque<std::pair<nana::string, bool> > >().swap(files_);
		
		using namespace path;
		files_.resize(tasks::count);

		implment<tasks::dllcache>::job(*this, m);
		implment<tasks::prefetch>::job(*this, m);
		implment<tasks::user_temporary>::job(*this, m);
		implment<tasks::windows_temporary>::job(*this, m);
		implment<tasks::internet_temporary>::job(*this, m);

		implment<tasks::wer_user_archive>::job(*this, m);
		implment<tasks::wer_user_queue>::job(*this, m);
		implment<tasks::wer_system_archive>::job(*this, m);
		implment<tasks::wer_system_queue>::job(*this, m);

		ready_ = true;
	}
	
	void clear(messenger& msnger)
	{
		std::vector<std::deque<std::pair<nana::string, bool> > >::iterator it = files_.begin(), end = files_.end();
		for(; it != end; ++it)
		{
			std::deque<std::pair<nana::string, bool> >::iterator file = it->begin(), last = it->end();
			for(; file != last; ++file)
			{
				if(file->second)
					nana::filesystem::rmdir(file->first.c_str(), true);
				else
					nana::filesystem::rmfile(file->first.c_str());
				msnger.active();
				msnger.message(file->first);
			}	
		}
		ready_ = false;
	}
	
	bool		ready() const			{	return ready_;	}
	std::size_t	bytes() const {	return bytes_;	}
	std::size_t	size() const	{	return size_;	}
	void reset()				{	ready_ = false;	}
private:
	struct helper
	{
		helper(const nana::string& path, messenger& m)
			:path(path + STR('\\')), bytes(0), size(0), msnger_(m)
		{}

		void operator()(const file_info_type& v)
		{
			nana::string file = path + v.name;
			if(v.directory)
			{
				helper hlp(file, msnger_);
				std::for_each<file_iterator, helper&>(file_iterator(hlp.path), file_iterator(), hlp);
				bytes += hlp.bytes;
				size += hlp.size;
				files.insert(files.end(), hlp.files.begin(), hlp.files.end());
			}
			else
				bytes += nana::filesystem::filesize(file.c_str());
			
			files.push_back(std::make_pair(file, v.directory));
			msnger_.active();

			++size;
		}

		nana::string	path;
		std::size_t		bytes;
		std::size_t		size;
		std::deque<std::pair<nana::string, bool> > files;
	private:
		messenger& msnger_;
	};

	template<int ID>
	struct implment
	{
		static void job(junk_sweeper& js, messenger& msnger)
		{
			nana::string path = path::creator<ID>::fetch();
			if(path.size())
			{
				nana::string::size_type envbeg = path.find('%');
				if(envbeg != path.npos)
				{
					nana::string::size_type envend = path.find('%', envbeg + 1);
					if(envend != path.npos)
					{
						nana::char_t buf[MAX_PATH];
						DWORD n = ::GetEnvironmentVariable(path.substr(envbeg + 1, envend - envbeg - 1).c_str(), buf, MAX_PATH);
						path.replace(envbeg, envend-envbeg + 1, buf);
					}
				}
				
				msnger.message(path::creator<ID>::msg());
				helper hlp(path, msnger);
				std::for_each<file_iterator, helper&>(file_iterator(path), file_iterator(), hlp);

				js.files_[ID].swap(hlp.files);
				
				js.bytes_ += hlp.bytes;
				js.size_ += hlp.size;
			}
		}
	};

private:
	bool ready_;
	std::size_t bytes_;
	std::size_t size_;
	std::vector<std::deque<std::pair<nana::string, bool> > > files_;
};

class frm_main: public nana::gui::form
{
	typedef frm_main self_type;
public:
	typedef nana::gui::appear appear;
	
	frm_main()
		: nana::gui::form(nana::gui::API::make_center(400, 350), appear::decorate<appear::minimize, appear::taskbar>())
	{
		this->caption(STR("The FreeMe - A Sample of Nana, stdex.sf.net"));
		
		pic_.create(*this, nana::rectangle(0, 0, 400, 144));
		pic_.load(STR("background.png"));

		desc_.create(*this, nana::rectangle(10, 150, 380, 110));
		desc_.caption(	STR("The FreeMe - A Sample of Nana C++ Library\n")
						STR("Refer to stdex.sourceforge.net for the source code if you are a C++ developer.\n\n")
						STR("The FreeMe cleans junk files in:\n    DLLCache, Prefetch, Temporary and Internet Cache Directories."));

		btn_.create(*this, nana::rectangle(270, 270, 120, 26));
		btn_.caption(STR("Scan junk files"));
		btn_.make_event<nana::gui::events::click>(*this, &self_type::_m_button);
		
		pgbar_.create(*this, nana::rectangle(0, 310, 400, 20));
		
		lbl_.create(*this, nana::rectangle(5, 335, 400, 15));
		lbl_.caption(STR("Nana C++ Library"));
		this->show();
	}
private:
	class pgbar_analysis_msnger: public messenger
	{
	public:
		pgbar_analysis_msnger(nana::gui::progress& pgbar, nana::gui::label& lbl)
			:pgbar_(pgbar), lbl_(lbl)
		{}

		void active()
		{
			if((size_ % 50) == 0)
			{
				nana::threads::thread::check_break(0);
				pgbar_.inc();
			}
			size_++;
		}
		
		void message(const nana::string& msg)
		{
			lbl_.caption(STR("Analyze...") + msg);
		}
	private:
		unsigned size_;
		nana::gui::progress& pgbar_;
		nana::gui::label& lbl_;
	};
	
	class pgbar_delete_msnger: public messenger
	{
	public:
		pgbar_delete_msnger(nana::gui::progress& pgbar, nana::gui::label& lbl)
			:pgbar_(pgbar), lbl_(lbl)
		{}

		void active()
		{
			nana::threads::thread::check_break(0);
			pgbar_.inc();
		}
		
		void message(const nana::string& msg)
		{
			lbl_.caption(STR("Deleting ") + msg);	
		}
	private:
		nana::gui::progress& pgbar_;
		nana::gui::label&	lbl_;
	};
private:
	void _m_button()
	{
		if(jkswp_.ready())
		{
			pgbar_.amount(static_cast<unsigned int>(jkswp_.size()));
			btn_.enabled(false);
			//Start a thread for deleting the files.
			thread_.start(nana::functor<void()>(*this, &self_type::_m_clear));
		}
		else
		{
			pgbar_.style(false);
			btn_.enabled(false);
			lbl_.caption(STR("Analyzing..."));
			//Start a thread for searching junk files
			thread_.start(nana::functor<void()>(*this, &self_type::_m_search));
		}
	}

	void _m_search()
	{
		const char* capa[] = {"bytes", "KB", "MB", "GB"};

		pgbar_analysis_msnger msnger(pgbar_, lbl_);
		jkswp_.search(msnger);

		pgbar_.style(true);
		pgbar_.value(0);

		nana::wint_t bytes = jkswp_.bytes();
		size_t x = 1;
		int capid = 0;

		while(bytes > (x * 1024) && capid < 3)
		{
			x *= 1024;
			++capid;
		}

		double capsize = double(bytes) / x;

		std::stringstream ss;
		ss<<capsize;
		std::string cap = ss.str();

		std::string::size_type dotpos = cap.find('.');
		if(dotpos != cap.npos)
		{
			std::string::size_type spare = cap.size() - dotpos - 1;
			if(spare > 2)
			{
				spare = 2;
				cap.erase(dotpos + 2);
			}

			for(std::string::size_type i = 0; i < spare; ++i)
			{
				if(cap[cap.size() - 1] == '0')
					cap.erase(cap.size() - 1);
			}

			if(cap[cap.size() - 1] == '.')
				cap.erase(cap.size() - 1);
		}

		btn_.enabled(true);
		ss.str("");
		ss.clear();
		ss<<"Analysis Result: "<<jkswp_.size()<<" files, "<<cap<<" "<<capa[capid]<<'.';
		lbl_.caption(nana::charset(ss.str()));
		btn_.caption(STR("FreeMe"));
	}
	
	void _m_clear()
	{
		pgbar_delete_msnger msnger(pgbar_, lbl_);
		jkswp_.clear(msnger);
		
		jkswp_.reset();
		btn_.caption(STR("Scan junk files"));
		btn_.enabled(true);
		lbl_.caption(STR("Done!"));
	}
private:
	nana::gui::picture pic_;
	nana::gui::label	desc_;
	nana::gui::button	btn_;
	nana::gui::progress pgbar_;
	nana::gui::label	lbl_;
	junk_sweeper		jkswp_;
	nana::threads::thread thread_;
};

/*
 * WinMain
 * @brief: Main Entry
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	frm_main main;
	nana::gui::exec();

	return 0;
}
