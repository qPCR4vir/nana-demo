#include <nana/deploy.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/treebox.hpp>

#include <nana/filesystem/filesystem_ext.hpp>
//#define NANA_USING_STD_FILESYSTEM      // if used this way, make sure nana was compiled with this option too
//#if 1 //NANA_USING_STD_FILESYSTEM

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

	// Boost can throw an exception "Access is denied"
	// when accessing some system paths, like "C:\Config.Msi"
	try {
	for (const auto& dir : SubDirectories{ fs_ext::def_rootstr })
	{
		if (! fs::is_directory(dir) ) continue;
		tree.insert(node, dir.path().filename().generic_string(), dir.path().filename().generic_string());
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

			auto child = tree.insert(arg.item, dir.path().filename().generic_string(),
				                               dir.path().filename().generic_string());
			if (child.empty()) continue;

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			try {
			for (const auto& dr : SubDirectories{ dir.path() })
			{
				if (!fs::is_directory(dr)) continue; //If it is not a directory.
				tree.insert(child, dr.path().filename().generic_string(),
					               dr.path().filename().generic_string());
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