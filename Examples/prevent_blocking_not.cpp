#include <iostream>

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/system/platform.hpp>
#include <nana/gui/widgets/progress.hpp>

class example : public nana::form
{
private:
    bool            working_   { false };
    nana::button    btn_start_ { *this, nana::rectangle( 10, 10, 100, 20)};
    nana::button    btn_cancel_{ *this, nana::rectangle(120, 10, 100, 20)};
    nana::progress  prog_      { *this, nana::rectangle( 10, 40, 280, 20)};

public:
    example()
    {
        caption("Not prevent blocking");
        btn_start_.caption("Start");
        btn_start_.events().click([this]() {_m_start(); });

        btn_cancel_.caption("Cancel");
        btn_cancel_.events().click([this]() { _m_cancel(); });
    }
private:
    void _m_start()
    {
        working_ = true;
        btn_start_.enabled(false);
        int amount = 10;
        prog_.amount(amount);
        for (int i = 0; i < amount && working_; ++i)  // amount s working
        {
            nana::system::sleep(1000); // 1 sec., a long-running simulation
            prog_.value(i + 1);        // why not refresh  ???
            std::cout << i;
        }
        btn_start_.enabled(true);
    }
    void _m_cancel() { working_ = false; }
};

int main()
{
    example ex;
    ex.show();
    nana::exec();
    return 0;
}

