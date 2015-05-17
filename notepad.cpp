
/** @file notepad.cpp Demo: Creating A Simple Notepad In Nana 0.8

Let's start a tutorial to create a simple notepad, the simple notepad is a text editor allows the user to select and browse existing text files. This example also shows how you can use place, menubar, menu, textbox, msgbox, and filebox and their various options.

This example requires Nana 0.8 for C++11 and a C++11 compiler.

Get Started

First of all, the whole program code is ready, and then we will go through each function.
*/


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
    place   place_;
    menubar menubar_;
    textbox textbox_;

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

};

int main()
{
    notepad_form npform;
    npform.show();
    exec();
}

/**
Simple Notepad

_m_pick_file()
We start with a private member function _m_pick_file(), this function is to tell user to select a file.

return (fbox.show() ? fbox.file() : nana::string());

If user clicks "cancel" button or closes the dialog by clicking 'X' close button, fbox.show() returns false for no file selection.

_m_ask_save()
This function will have asked user to save the text to a file by the time the text is closed.

if(textbox_.edited())

Determines whether the text has been edited. If there are modifications to the text, then it

auto fs = textbox_.filename();

When the textbox opens a file or saves a text to a file, the textbox will keep the filename. If fs is empty, the program asks user to select a file to save the text.

_m_ask_save() has a return type, that is bool type. And it returns false if and only if the user cancel the selection.

notepad_form()
In the default of constructor, we need create the menubar and textbox, and set the layout for the form.

textbox_.borderless(true);
API::effects_edge_nimbus(textbox_, effects::edge_nimbus::none);

Disables the border and edge numbus effect of the textbox.

textbox_.events().mouse_dropfiles([this](const arg_drppfiles& arg)
{
    if (arg.files.size() && _m_ask_save())
        textbox_.load(arg.files.at(0).data());
});

Sets a Drag'n Drop event for the textbox, it allows user to open a file by dragging the file outside of the program and dropping the file inside the program. the call of _m_ask_save() here is to try to ask user to save the edited text.

events().unload([this](const arg_unload& arg){
    if (!_m_ask_save())
        arg.cancel = true;
});

Sets an unload event for the form, it enables program to ask user to save the edited text when closing the program, and if user cancels the messagebox, the program stops closing.

_m_make_menus()
Sets menus for the menubar.

int main()
Creates the form of notpad.

*/

