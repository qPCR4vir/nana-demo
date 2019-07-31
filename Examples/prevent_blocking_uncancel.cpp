// need nana 1.7.2
#include <iostream>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/system/platform.hpp>
#include <nana/gui/widgets/progress.hpp>
#include <nana/threads/pool.hpp>

class example : public nana::form
{
private:
    bool            working_   { false };
    nana::button    btn_start_ { *this, nana::rectangle( 10, 10, 100, 20)};
    nana::progress  prog_      { *this, nana::rectangle( 10, 40, 280, 20)};
    nana::threads::pool pool_;

public:
    example()
    {
        caption("Prevent blocking");
        btn_start_.caption("Start");
        btn_start_.events().click(nana::threads::pool_push(pool_, *this, &example::_m_start));
        btn_start_.events().click(nana::threads::pool_push(pool_, *this, &example::_m_ui_update));

        this->events().unload([this](const nana::arg_unload& ei) { _m_cancel(ei); });
    }
private:
    void _m_start()
    {
        btn_start_.enabled(false);
        nana::system::sleep(1000); // 1 sec., a long-running simulation
        btn_start_.enabled(true);
    }
    void _m_ui_update()
    {
        while(!btn_start_.enabled())
        {
            prog_.inc();
            nana::system::sleep(100);
        }
    }
    void _m_cancel(const nana::arg_unload& ei)
    {
        if(!btn_start_.enabled())
            ei.cancel = true;
    }
};

int main()
{
    example ex;
    ex.show();
    nana::exec();
    return 0;
}

