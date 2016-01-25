#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/file_iterator.hpp>

int main()
{
	using namespace nana;
	form fm{ API::make_center(300, 300), appear::decorate<appear::taskbar>() };
	fm.caption("Nana C++ Library - Treebox Sample.");

	nana::treebox tree(fm, { 0, 0, 300, 300 });

#if defined(NANA_WINDOWS)
	auto node = tree.insert("C:", "Local Drive(C:)");
	filesystem::file_iterator i("C:\\"), end;
#elif defined(NANA_LINUX)
	auto node = tree.insert("/", "Root/");
	filesystem::file_iterator i("/"), end;
#endif
	for (; i != end; ++i)
	{
		if (!i->directory) continue;
		tree.insert(node, i->name, i->name);
		break;
	}

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto path = tree.make_key_path(arg.item, "/") + "/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		//Walk in the path directory for sub directories.
		for (filesystem::file_iterator i{ path }, end; i != end; ++i)
		{
			if (!i->directory) continue; //If it is not a directory.

			auto child = tree.insert(arg.item, i->name, i->name);
			if (child.empty()) continue;

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			for (filesystem::file_iterator u{ path + i->name }; u != end; ++u)
			{
				if (!u->directory) continue; //If it is not a directory.
				tree.insert(child, u->name, u->name);
				break;
			}
		}
		tree.auto_draw(true);
	});

	fm.show();
	exec();
}