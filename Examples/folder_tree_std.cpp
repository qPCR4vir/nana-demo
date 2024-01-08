#include <nana/deploy.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/treebox.hpp>

#include <nana/filesystem/filesystem_ext.hpp>

int main()
{
	using namespace nana;
	namespace fs = std::filesystem;
	namespace fs_ext = nana::filesystem_ext;
	using SubDirectories = fs::directory_iterator;

	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption("Nana C++ Library - Treebox-std::filesystem example.");
	nana::treebox tree{ fm,{ 10, 10, 380, 480 } };

	auto node = tree.insert(fs_ext::def_root, fs_ext::def_rootname);

	// Boost? can throw an exception "Access is denied"
	// when accessing some system paths, like "C:\Config.Msi"
	try {
		// like fs_ext::directory_only_iterator& find_first() to build the root node
		for (const auto& dir : SubDirectories{ fs_ext::def_rootstr })
		{
			if (! fs::is_directory(dir) ) continue;
            std::string dir_name = dir.path().filename().generic_string();
			tree.insert(node, dir_name, dir_name);
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
		for (const auto& dir : SubDirectories{ Path })
		{
			if (!fs::is_directory(dir)) continue; //If it is not a directory.
			std::string dir_name = dir.path().filename().generic_string();
			auto child = tree.insert(arg.item, dir_name, dir_name);
			if (child.empty()) continue;  // ?

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
		try {
			for (const auto& dr : SubDirectories{ dir.path() })
			{
				if (!fs::is_directory(dr)) continue; //If it is not a directory.
				std::string sdir_name = dir.path().filename().generic_string();
				tree.insert(child, sdir_name, sdir_name);
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
/*		2, 1, [&node]()
	{
		node.expand(true);
	}
*/
#endif

	);
}
//#endif