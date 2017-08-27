#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
//#define NANA_FILESYSTEM_FORCE 
#include <nana/filesystem/filesystem_ext.hpp>


int main()
{
	using namespace nana;
	namespace fs = std::experimental::filesystem;
	namespace fs_ext = nana::filesystem_ext;
	using dir_it = fs_ext::directory_only_iterator;

	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption("Nana C++ Library - Treebox-nana::experimental::filesystem example.");
	nana::treebox tree{ fm, { 10, 10, 380, 480 } };
    
	auto node = tree.insert(fs_ext::def_root, fs_ext::def_rootname);

	dir_it sub_root{ fs_ext::def_rootstr};
	auto p = sub_root->path();
	std::string dir_name=p.filename().generic_u8string();
	tree.insert(node, dir_name,dir_name);

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto Path = tree.make_key_path(arg.item, "/") + "/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		//Walk in the path directory for sub directories.
		for (const auto& dir : dir_it{ Path })
		{
			std::string dir_name=dir.path().filename().generic_u8string();

			auto child = tree.insert(arg.item, dir_name, dir_name);
			if (child.empty()) continue;   // ?

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			dir_it d{ dir.path() };
			if (d != dir_it{})
			{
				std::string sdir_name = d->path().filename().generic_u8string();
				tree.insert(child, sdir_name, sdir_name);
			}
		}
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