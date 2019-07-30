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
#ifdef NANA_WINDOWS
#ifdef BUILD_FreeMe

#include <nana/gui.hpp>
#include <shlobj.h>
#include <windows.h>
using wstring = std::wstring;

namespace path
{
	namespace detail
	{
		 wstring reg_string(HKEY hkey, const wchar_t * subkey, const  wchar_t * value)
		{
			HKEY key;
			std::wstring str;
			if(::RegOpenKey(hkey, subkey, &key) != ERROR_SUCCESS  )
				return str;
			
			DWORD len = MAX_PATH;
			auto buf = std::make_unique< wchar_t[]>( len );

			LONG result;
			while(ERROR_MORE_DATA == (result = ::RegQueryValueEx(key, value, 0, 0, reinterpret_cast<LPBYTE>(buf.get()), &len)))
				buf = std::make_unique< wchar_t[]>( len );

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
		static wstring fetch(); ///< the dir path
		static wstring msg();
	};
	
	template<>
	struct creator<tasks::dllcache>
	{
		static wstring fetch()
		{
			wchar_t temp[260];
			::GetSystemDirectory(temp, 260);
			return wstring(temp) +  (L"\\dllcache");
		}
		
		static wstring msg()
		{
			return  (L"DLLCache");	
		}
	};

	template<>
	struct creator<tasks::prefetch>
	{
		static wstring fetch()
		{
			wchar_t temp[260];
			::GetWindowsDirectory(temp, 260);
			return wstring(temp) +  (L"\\Prefetch");
		}
		
		static wstring msg()
		{
			return  (L"Prefetch");	
		}
	};

	template<>
	struct creator<tasks::user_temporary>
	{
		static wstring fetch()
		{
			wchar_t temp[260];
			::GetTempPath(260, temp);
			return wstring(temp).substr(0,  std::wcslen(temp) - 1);
		}
		
		static wstring msg()
		{
			return  (L"Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::windows_temporary>
	{
		static wstring fetch()
		{
			wchar_t temp[260];
			::GetWindowsDirectory(temp, 260);
			return wstring(temp) +  (L"\\Temp");
		}
		
		static wstring msg()
		{
			return  (L"Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::internet_temporary>
	{
		static wstring fetch()
		{
			wchar_t temp[MAX_PATH];
			::SHGetFolderPath(0, CSIDL_INTERNET_CACHE, 0, 0, temp);
			return wstring(temp);
		}
		
		static wstring msg()
		{
			return (L"Internet Temporary Directory");	
		}
	};

	template<>
	struct creator<tasks::wer_user_archive>
	{
		static wstring fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Archive Files"), 
				(L"Folder"));
		}

		static wstring msg()
		{
			return (L"Windows Error Reporting Archive Files");
		}
	};

	template<>
	struct creator<tasks::wer_user_queue>
	{
		static wstring fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting Queue Files"), 
				(L"Folder"));
		}

		static wstring msg()
		{
			return (L"Windows Error Reporting Queue Files");
		}
	};

	template<>
	struct creator<tasks::wer_system_archive>
	{
		static wstring fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Archive Files"), 
				(L"Folder"));
		}

		static wstring msg()
		{
			return (L"Windows Error Reporting System Archive Files");
		}
	};

	template<>
	struct creator<tasks::wer_system_queue>
	{
		static wstring fetch()
		{
			return detail::reg_string(HKEY_LOCAL_MACHINE, 
				(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\VolumeCaches\\Windows Error Reporting System Queue Files"), 
				(L"Folder"));
		}

		static wstring msg()
		{
			return (L"Windows Error Reporting System Queue Files");
		}
	};
}



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
//#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>
#include <nana/filesystem/filesystem_ext.hpp>

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
	virtual void message(const wstring&){}
	virtual bool stopped() {return false;}
};

/// search temporary files and clear these files.
class junk_sweeper
{
	bool		ready_{ false };
	std::size_t bytes_{};
	std::size_t size_ {};
	std::vector<std::deque<std::pair<wstring, bool> > > files_;

	using self_type = junk_sweeper ;

 public:
	 using file_iterator = std::experimental::filesystem::directory_iterator; // ::file_iterator file_iterator;
	 using file_info_type =  file_iterator::value_type       ;

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
					// nana::experimental::filesystem::rmdir(file.first.c_(L), true);
					std::wcout << (L"Deleting directory: ") << file.first << (L"\n");
				}
				else
				{
					// nana::experimental::filesystem::rmfile(file.first.c_(L));
					std::wcout << (L"Deleting file: ") << file.first << (L"\n");
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
		wstring	path;
		std::size_t		bytes;
		std::size_t		size;
		std::deque<std::pair<wstring, bool> > files;
	 private:
		messenger& msnger_;
	 public:
		helper(const wstring& path, messenger& m)
			:path(path + (L'\\')), bytes(0), size(0), msnger_(m)
		{}

		void operator()(const file_info_type& v)
		{
			wstring file = path + v.name;
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
				bytes += nana::experimental::filesystem::filesize(file.c_str());
			
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
			wstring path = path::creator<ID>::fetch();
			if ( path.empty() )  return;
			wstring::size_type envbeg = path.find('%');
			if(envbeg != path.npos)
			{
				wstring::size_type envend = path.find('%', envbeg + 1);
				if(envend != path.npos)
				{
					wchar_t buf[MAX_PATH];
					DWORD n = ::GetEnvironmentVariable(path.sub(Lenvbeg + 1, envend - envbeg - 1).c_(L), buf, MAX_PATH);
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
		(L"The FreeMe - A Sample of Nana C++ Library GUI programing\n")
		(L"Refer to http://nanapro.org/ for the source code if you are a C++ developer.\n\n")
		(L"(Currently it don't erase any file, only show what it could delete in a real version)\n")
		(L"The FreeMe cleans WINDOWS from junk files in:\n    DLLCache, Prefetch, Temporary and Internet Cache Directories.") };
																 
	nana::button   scan_btn_{ *this, (L"Scan junk files") };	 
	nana::progress	pgbar_	{ *this };							 
	nana::label		lbl_	{ *this, (L"Nana C++ Library") };  
	nana::picture	pic_	{ *this };							 
	junk_sweeper	jkswp_;  // dont do anything
	std::thread		thread_; // ?

	typedef frm_main self_type;
public:
	typedef nana::appear appear;
	
	frm_main()
		: nana::form(nana::API::make_center(400, 350), appear::decorate<appear::minimize, appear::taskbar>())
	{
		this->caption((L"The FreeMe - A Sample of Nana, stdex.sf.net"));
		
		pic_.load((L"..\\Examples\\image02.bmp"   /*background.png"*/   ));

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
		
		void message(const wstring& msg) override
		{
			lbl_.caption((L"Analyzing...") + msg );
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
		
		void message(const wstring& msg)
		{
			lbl_.caption((L"Deleting ") + msg);	
		}
		bool stopped() override
		{
			return pgbar_.stopped();
		}

	};
private:
	void _m_button()
	{
		if (scan_btn_.caption() == (L"Stop"))
			pgbar_.stop();
		else
		if(jkswp_.ready())
		{
			pgbar_.stop(false);
			pgbar_.unknown(false);
			pgbar_.amount(static_cast<unsigned int>(jkswp_.size()));
			scan_btn_.caption((L"Stop"));
			//Start a thread for deleting the files.
            std::thread ([this](){_m_clear();}).detach();
		}
		else
		{
			pgbar_.stop(false);
			pgbar_.unknown(true);
		    pgbar_.value(0);
			scan_btn_.caption((L"Stop"));
			lbl_.caption((L"Scaning..."));
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
		ss.(L"");
		ss.clear();
		ss << cap << " " << capa[capid];
		return ss.(L);
	}

	void _m_search()
	{
		pgbar_analysis_msnger msnger(pgbar_, lbl_);
		jkswp_.search(msnger);

		uintmax_t bytes = jkswp_.bytes();
		scan_btn_.caption((L"FreeMe"));
		std::stringstream ss;
		ss << (pgbar_.stop(false) ? "Stoped at: " : "Analysis Result: ")<<jkswp_.size()<<" files, ["<<cap(bytes)<<"].";
		lbl_.caption(wstring(nana::charset(ss.(L))));
	}
	
	void _m_clear()
	{
		pgbar_delete_msnger msnger(pgbar_, lbl_);
		jkswp_.clear(msnger);
		
		jkswp_.reset();
		scan_btn_.caption((L"Scan junk files"));
		lbl_.caption( (pgbar_.stop(false) ? (L"Stoped!") : (L"Done!") )  ) ;
	}
};

int  main( )
{
	frm_main main;
	nana::exec();

	return 0;
}
#endif // BUILD_FreeMe
#endif // NANA_WINDOWS
