

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/audio/player.hpp>
#include <nana/threads/pool.hpp>

int main()
{
    nana::audio::player player(STR("audio.wav"));
    nana::threads::pool pool(1); //Only 1 thread.

    using namespace nana;

    form fm;
    button btn(fm, nana::rectangle(10, 10, 100, 24));
    btn.caption(STR("Play the audio"));

    //C++11
    btn.events().click(pool_push(pool, [&]()
                           {
                               player.play();
                           }));

    fm.show();

    exec();
}
