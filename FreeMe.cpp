/**
 *	The FreeMe - A sample of Nana C++ Library
 *	Copyright(C) 2007-2009 Jinhao
 *
 *	Distributed under the Nana Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://stdex.sourceforge.net/LICENSE_1_0.txt)
 *
 *	@description:
 *		This sample shows you how to write an application with Nana C++ Library.
 *    This example need update. 
 *   \todo update to nana v1.0.2
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
			if(::RegOpenKey(hkey, subkey, &key) != ERROR_SUCCESS  )
				return str;
			
			DWORD len = MAX_PATH;
			auto buf = std::make_unique< nana::char_t[]>( len );

			LONG result;
			while(ERROR_MORE_DATA == (result = ::RegQueryValueEx(key, value, 0, 0, reinterpret_cast<LPBYTE>(buf.get()), &len)))
				buf = std::make_unique< nana::char_t[]>( len );

			if(ERROR_SUCCESS == result)
				str = buf.get();

			return str;
		}
	}
	struct tasks
	{
		enum { dllcache,         prefetch,       user_temporary,     windows_temporary,  internet_temporary,
			   wer_user_archive, wer_user_queue, wer_system_archive, wer_system_queue,
	 		   count  };

	};
	
	template<int ID>
	struct creator
	{
		static nana::string fetch(); ///< the dir path
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
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Archive Files"), 
				STR("Folder"));
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
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Queue Files"), 
				STR("Folder"));
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
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Archive Files"), 
				STR("Folder"));
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
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				STR("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Queue Files"), 
				STR("Folder"));
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
//#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <nana/filesystem/file_iterator.hpp>
#include <nana/filesystem/fs_utility.hpp>
#include <thread>
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
//#include "path_creator.hpp"
	
/// to deliver a message from junk_sweeper to other classes.
class messenger
{
  public:
	virtual ~messenger(){}
	virtual void active(){}
	virtual void message(const nana::string&){}
	virtual bool stopped() {return false;}
};

/// search temporary files and clear these files.
class junk_sweeper
{
	bool		ready_{ false };
	std::size_t bytes_{};
	std::size_t size_ {};
	std::vector<std::deque<std::pair<nana::string, bool> > > files_;

	using self_type = junk_sweeper ;

 public:
	typedef nana::filesystem::file_iterator file_iterator;
	typedef file_iterator::value_type       file_info_type;

	junk_sweeper() {}

	void search(messenger& m)
	{
		size_ = 0;
		bytes_ = 0;
		ready_ = false;
		files_.clear();
		
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
		ready_ = false;
		for (auto &temp_dir : files_ )
			for(auto &file : temp_dir)
			{
				if (msnger.stopped()) 
					return;

				if (file.second)
				{
					// nana::filesystem::rmdir(file.first.c_str(), true);
					std::wcout << STR("Deleting directory: ") << file.first << STR("\n");
				}
				else
				{
					// nana::filesystem::rmfile(file.first.c_str());
					std::wcout << STR("Deleting file: ") << file.first << STR("\n");
				}
				msnger.active();            // inc progresbar
				msnger.message(file.first); // 
			}	
	}
	
	bool		ready() const	{	return ready_;	}
	std::size_t	bytes() const   {	return bytes_;	}
	std::size_t	size() const	{	return size_;	}
	void reset()				{	ready_ = false;	}
 private:
	struct helper
	{
		nana::string	path;
		std::size_t		bytes;
		std::size_t		size;
		std::deque<std::pair<nana::string, bool> > files;
	 private:
		messenger& msnger_;
	 public:
		helper(const nana::string& path, messenger& m)
			:path(path + STR('\\')), bytes(0), size(0), msnger_(m)
		{}

		void operator()(const file_info_type& v)
		{
			nana::string file = path + v.name;
			if(v.directory)
			{
				helper hlp(file, msnger_);
				for (auto & sub_dir = file_iterator(hlp.path); sub_dir != file_iterator(); ++sub_dir)
				{
					if (msnger_.stopped()) break;
					hlp(*sub_dir);
				}
				//std::for_each<file_iterator, helper&>(file_iterator(hlp.path), file_iterator(), hlp);
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
	};

	template<int ID>
	struct implment
	{
		static void job(junk_sweeper& js, messenger& msnger)
		{
			nana::string path = path::creator<ID>::fetch();
			if ( path.empty() )  return;
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

			js.files_[ID].swap(hlp.files); // crash here??
				
			js.bytes_ += hlp.bytes;
			js.size_ += hlp.size;
		}
	};
};

class frm_main: public nana::form
{
	nana::place     plc_	{ *this };
	nana::label		desc_   { *this,
		STR("The FreeMe - A Sample of Nana C++ Library GUI programing\n")
		STR("Refer to http://nanapro.org/ for the source code if you are a C++ developer.\n\n")
		STR("(Currently it don't erase any file, only show what it could delete in a real version)\n")
		STR("The FreeMe cleans WINDOWS from junk files in:\n    DLLCache, Prefetch, Temporary and Internet Cache Directories.") };
																 
	nana::button   scan_btn_{ *this, STR("Scan junk files") };	 
	nana::progress	pgbar_	{ *this };							 
	nana::label		lbl_	{ *this, STR("Nana C++ Library") };  
	nana::picture	pic_	{ *this };							 
	junk_sweeper	jkswp_;  // dont do anything
	std::thread		thread_; // ?

	typedef frm_main self_type;
public:
	typedef nana::appear appear;
	
	frm_main()
		: nana::form(nana::API::make_center(400, 350), appear::decorate<appear::minimize, appear::taskbar>())
	{
		this->caption(STR("The FreeMe - A Sample of Nana, stdex.sf.net"));
		
		pic_.load(STR("..\\Examples\\image02.bmp"   /*background.png"*/   ));

        scan_btn_.events().click([this](){_m_button();});
		
		plc_.div("vertical  gap=5 <weight=144 pic>"
			     "                <weight=110 desc>"
			     "                <weight=26 <> <weigth= 120 scan> >"
			     "                <weight=10  >"
			     "                <weight=20 pgbar>"
			     "                <weight=2  >"
			     "                <weight=15 lbl>"
			);
		plc_["pic" ] << pic_ ;
		plc_["desc"] << desc_;
		plc_["scan"] << scan_btn_;
		plc_["pgbar"]<< pgbar_;
		plc_["lbl" ] << lbl_;

		plc_.collocate();

		this->show();
	}
private:
	class pgbar_analysis_msnger: public messenger
	{
		unsigned		size_;
		nana::progress &pgbar_;
		nana::label    &lbl_;
	public:
		pgbar_analysis_msnger(nana::progress& pgbar, nana::label& lbl)
			:pgbar_(pgbar), lbl_(lbl)
		{}

		void active() override
		{
			if((size_ % 50) == 0)
			{
				//nana::threads::check_break(0);   ?????
				pgbar_.inc();
			}
			size_++;
		}
		
		void message(const nana::string& msg) override
		{
			lbl_.caption(STR("Analyzing...") + msg );
		}
		bool stopped() override
		{
			return pgbar_.stopped();
		}
	};
	
	class pgbar_delete_msnger: public messenger
	{
		nana::progress& pgbar_;
		nana::label&	lbl_;
	public:
		pgbar_delete_msnger(nana::progress& pgbar, nana::label& lbl)
			:pgbar_(pgbar), lbl_(lbl)
		{}

		void active()
		{
			//nana::threads::thread::check_break(0);
			pgbar_.inc();
		}
		
		void message(const nana::string& msg)
		{
			lbl_.caption(STR("Deleting ") + msg);	
		}
		bool stopped() override
		{
			return pgbar_.stopped();
		}

	};
private:
	void _m_button()
	{
		if (scan_btn_.caption() == STR("Stop"))
			pgbar_.stop();
		else
		if(jkswp_.ready())
		{
			pgbar_.stop(false);
			pgbar_.unknown(false);
			pgbar_.amount(static_cast<unsigned int>(jkswp_.size()));
			scan_btn_.caption(STR("Stop"));
			//Start a thread for deleting the files.
            std::thread ([this](){_m_clear();}).detach();
		}
		else
		{
			pgbar_.stop(false);
			pgbar_.unknown(true);
		    pgbar_.value(0);
			scan_btn_.caption(STR("Stop"));
			lbl_.caption(STR("Scaning..."));
			//Start a thread for searching junk files
            std::thread ([this](){_m_search();}).detach();
		}
	}

	static std::string cap(uintmax_t bytes)
	{
		const char* capa[] = {"bytes", "KB", "MB", "GB"};

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
		ss.str("");
		ss.clear();
		ss << cap << " " << capa[capid];
		return ss.str();
	}

	void _m_search()
	{
		pgbar_analysis_msnger msnger(pgbar_, lbl_);
		jkswp_.search(msnger);

		uintmax_t bytes = jkswp_.bytes();
		scan_btn_.caption(STR("FreeMe"));
		std::stringstream ss;
		ss << (pgbar_.stop(false) ? "Stoped at: " : "Analysis Result: ")<<jkswp_.size()<<" files, ["<<cap(bytes)<<"].";
		lbl_.caption(nana::string(nana::charset(ss.str())));
	}
	
	void _m_clear()
	{
		pgbar_delete_msnger msnger(pgbar_, lbl_);
		jkswp_.clear(msnger);
		
		jkswp_.reset();
		scan_btn_.caption(STR("Scan junk files"));
		lbl_.caption( (pgbar_.stop(false) ? STR("Stoped!") : STR("Done!") )  ) ;
	}
};

int  main( )
{
	frm_main main;
	nana::exec();

	return 0;
}
