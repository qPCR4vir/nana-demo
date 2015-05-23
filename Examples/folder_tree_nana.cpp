#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/filesystem.hpp>

int main()
{
	using namespace nana;
	using namespace experimental;

	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption(L"Nana C++ Library - Treebox-filesystem example.");

	nana::treebox tree{ fm, { 10, 10, 380, 480 } };

#if defined(NANA_WINDOWS)
	auto node = tree.insert(L"C:", L"Local Drive(C:)");
	filesystem::directory_iterator i(L"C:\\"), end;
#elif defined(NANA_LINUX)
	auto node = tree.insert(L"/", L"Root/");
	filesystem::directory_iterator i(L"/"), end;
#endif
	for (; i != end; ++i)
	{
		if (!i->attr.directory) continue;
		tree.insert(node, i->path().name(), i->path().name());
		break;
	}

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto Path = tree.make_key_path(arg.item, L"/") + L"/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		//Walk in the path directory for sub directories.
		for (filesystem::directory_iterator i{ Path }, end; i != end; ++i)
		{
			if (!i->attr.directory) continue; //If it is not a directory.

			auto child = tree.insert(arg.item, i->path().name(), i->path().name());
			if (child.empty()) continue;

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			for (filesystem::directory_iterator u{ Path + i->path().name() }; u != end; ++u)
			{
				if (!u->attr.directory) continue; //If it is not a directory.
				tree.insert(child, u->path().name(), u->path().name());
				break;
			}
		}
		tree.auto_draw(true);
	});

	fm.show();
	exec();
}