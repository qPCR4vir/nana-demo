#include <nana/gui.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/msgbox.hpp>
#include <nana/gui/filebox.hpp>

using namespace nana;

class notepad_form
    : public form
{
public:
    notepad_form()
    {
        caption(L"Simple Notepad - Nana C++ Library");
        menubar_.create(*this);

        textbox_.create(*this);
        textbox_.borderless(true);
        API::effects_edge_nimbus(textbox_, effects::edge_nimbus::none);
        textbox_.enable_dropfiles(true);
        textbox_.events().mouse_dropfiles([this](const arg_dropfiles& arg)
        {
            if (arg.files.size() && _m_ask_save())
                textbox_.load(arg.files.at(0).data());
        });

        _m_make_menus();

        place_.bind(*this);
        place_.div("vert<menubar weight=28><textbox>");
        place_.field("menubar") << menubar_;
        place_.field("textbox") << textbox_;
        place_.collocate();

        events().unload([this](const arg_unload& arg){
            if (!_m_ask_save())
                arg.cancel = true;
        });
    }
private:
    nana::string _m_pick_file(bool is_open) const
    {
        filebox fbox(*this, is_open);
        fbox.add_filter(L"Text", L"*.txt");
        fbox.add_filter(L"All Files", L"*.*");

        return (fbox.show() ? fbox.file() : nana::string());
    }

    bool _m_ask_save()
    {
        if (textbox_.edited())
        {
            auto fs = textbox_.filename();
            msgbox box(*this, L"Simple Notepad", msgbox::button_t::yes_no_cancel);
            box << L"Do you want to save these changes?";

            switch (box.show())
            {
            case msgbox::pick_yes:
                if (fs.empty())
                {
                    fs = _m_pick_file(false);
                    if (fs.empty())
                        break;
                    if (fs.find(L".txt") == fs.npos)
                        fs += L".txt";
                }
                textbox_.store(fs.data());
                break;
            case msgbox::pick_no:
                break;
            case msgbox::pick_cancel:
                return false;
            }
        }
        return true;
    }

    void _m_make_menus()
    {
        menubar_.push_back(L"&FILE");
        menubar_.at(0).append(L"New", [this](menu::item_proxy& ip)
        {
            if(_m_ask_save())
                textbox_.reset();
        });
        menubar_.at(0).append(L"Open", [this](menu::item_proxy& ip)
        {
            if (_m_ask_save())
            {
                auto fs = _m_pick_file(true);
                if (fs.size())
                    textbox_.load(fs.data());
            }
        });
        menubar_.at(0).append(L"Save", [this](menu::item_proxy&)
        {
            auto fs = textbox_.filename();
            if (fs.empty())
            {
                fs = _m_pick_file(false);
                if (fs.empty())
                    return;
            }
            textbox_.store(fs.data());
        });

        menubar_.push_back(L"F&ORMAT");
        menubar_.at(1).append(L"Line Wrap", [this](menu::item_proxy& ip)
        {
            textbox_.line_wrapped(ip.checked());
        });
        menubar_.at(1).check_style(0, menu::checks::highlight);
    }
private:
    place   place_;
    menubar menubar_;
    textbox textbox_;
};

int main()
{
    notepad_form npform;
    npform.show();
    exec();
}