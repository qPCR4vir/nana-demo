#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/filesystem.hpp>
#include <nana/filesystem/filesystem_ext.hpp>



int main()
{
	using namespace nana;
	using namespace nana::experimental;
	using SubDirectories = filesystem::directory_iterator;
	using namespace nana::experimental::filesystem::ext;
	using dir_it = directory_only_iterator<filesystem::directory_iterator>;


	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption("Nana C++ Library - Treebox-nana::experimental::filesystem example.");

	nana::treebox tree{ fm, { 10, 10, 380, 480 } };
    
	auto node = tree.insert(def_root, def_rootname);

	dir_it sub_root{def_rootstr};
	std::string dir_name=sub_root->path().filename().generic_u8string();
	tree.insert(node, dir_name,dir_name);

//	for (const auto& dir : SubDirectories{ def_rootstr })
//	{
//		if (!filesystem::is_directory(dir)) continue;
//		tree.insert(node, dir.path().filename().generic_u8string(),
//			dir.path().filename().generic_u8string());
//		break;
//	}

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto Path = tree.make_key_path(arg.item, "/") + "/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		//Walk in the path directory for sub directories.
		for (const auto& dir : SubDirectories{ Path })
		{
			if (!filesystem::is_directory(dir)) continue; //If it is not a directory.

			auto child = tree.insert(arg.item, dir.path().filename().generic_u8string(),
				dir.path().filename().generic_u8string());
			if (child.empty()) continue;

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			for (const auto& dr : SubDirectories{  dir.path() })
			{
				if (!filesystem::is_directory(dr)) continue; //If it is not a directory.
				tree.insert(child, dr.path().filename().generic_u8string(),
					dr.path().filename().generic_u8string());
				break;
			}
		}
		tree.auto_draw(true);
	});

	fm.show();
	exec();
}