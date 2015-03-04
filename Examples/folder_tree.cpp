#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/file_iterator.hpp>


class folder_tree
    : public nana::gui::form
{
public:
    typedef nana::gui::treebox::item_proxy item_proxy;

    folder_tree()
        : nana::gui::form(nana::gui::API::make_center(300, 300),
                          appear::decorate<appear::taskbar>())
    {

        tree_.create(*this, nana::rectangle(0, 0, 300, 300));

#if defined(NANA_WINDOWS)
        item_proxy node = tree_.insert(STR("C:"), STR("Local Drive(C:)"));
        nana::filesystem::file_iterator i(STR("C:\\")), end;
#elif defined(NANA_LINUX)
        //Use a whitespace for the root key since the root under linux
        //is / character.
        item_proxy node = tree_.insert(STR("/"), STR("Root"));
        nana::filesystem::file_iterator i(STR("/")), end;
#endif
        for(; i != end; ++i)
        {
            if(false == i->directory) continue;

            tree_.insert(node, i->name, i->name);
            break;
        }
        tree_.ext_event().expand = nana::make_fun(*this, &folder_tree::_m_expand);
    }
private:
    void _m_expand(nana::gui::window, item_proxy node, bool exp)
    {
        if(!exp) return; //If this is contracted.

        //Windows supports the path separator '/'
        nana::string path = tree_.make_key_path(node, STR("/")) + STR("/");

        //Trim the whitespace at head of the path. So the code can be work
        //well in both Windows and Linux.
        nana::string::size_type path_start_pos = path.find_first_not_of(STR(" "));
        if(path_start_pos != path.npos)
            path.erase(0, path_start_pos);

        //Walk in the path directory for sub directories.
        nana::filesystem::file_iterator i(path), end;
        for(; i != end; ++i)
        {
            if(false == i->directory) continue; //If it is not a directory.

            item_proxy child = tree_.insert(node, i->name, i->name);
            if(child.empty()) continue;

            //Find a directory in child directory, if there is a directory,
            //insert it into the child, just insert one node to indicate the
            //node has a child and an arrow symbol will be displayed in the
            //front of the node.
            nana::filesystem::file_iterator u(path + i->name);
            for(; u != end; ++u)
            {
                if(false == u->directory) continue; //If it is not a directory.
                tree_.insert(child, u->name, u->name);
                break;
            }
        }
    }
private:
    nana::gui::treebox tree_;
};

int main()
{
    folder_tree fdtree;
    fdtree.show();
    nana::gui::exec();
}

