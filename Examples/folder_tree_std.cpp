#include <nana/deploy.hpp>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/filesystem_ext.hpp>

// NOTE: boost::filesystem::path doesn't have a generic_u8string method,
// so we are using nana::to_utf8(generic_wstring) in this demo.

int main()
{
	using namespace nana;
	namespace fs = std::experimental::filesystem;
	namespace fs_ext = nana::filesystem_ext;
	using SubDirectories = fs::directory_iterator;

	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption("Nana C++ Library - Treebox-nana::experimental::filesystem example.");

	nana::treebox tree{ fm,{ 10, 10, 380, 480 } };

	auto node = tree.insert(fs_ext::def_root, fs_ext::def_rootname);

	// Boost can throw an exception "Access is denied"
	// when accessing some system paths, like "C:\Config.Msi"
	try {
		for (SubDirectories dir(fs_ext::def_rootstr); dir != SubDirectories(); ++dir)
		{
			if (!fs::is_directory(*dir)) continue;
			tree.insert(node, nana::to_utf8(dir->path().filename().generic_wstring()),
							nana::to_utf8(dir->path().filename().generic_wstring()));
			break;
		}
	} catch (...) {}

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto Path = tree.make_key_path(arg.item, "/") + "/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		try {
			//Walk in the path directory for sub directories.
			for (SubDirectories dir(Path); dir != SubDirectories(); ++dir)
			{
				if (!fs::is_directory(*dir)) continue;

				auto child = tree.insert(arg.item, nana::to_utf8(dir->path().filename().generic_wstring()),
												nana::to_utf8(dir->path().filename().generic_wstring()));
				if (child.empty()) continue;

				//Find a directory in child directory, if there is a directory,
				//insert it into the child, just insert one node to indicate the
				//node has a child and an arrow symbol will be?displayed in the
				//front of the node.
				try {
					for (SubDirectories subdir(dir->path()); subdir != SubDirectories(); ++subdir)
					{
						if (!fs::is_directory(*subdir)) continue;
						tree.insert(child, nana::to_utf8(subdir->path().filename().generic_wstring()),
										nana::to_utf8(subdir->path().filename().generic_wstring()));
						break;
					}
				} catch (...) {}
			}
		} catch (...) {}
		tree.auto_draw(true);
	});

	fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING
		2, 1, [&node]()
	{
		node.expand(true);
	}
#endif

	);
}
