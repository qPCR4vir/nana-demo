#include <nana/audio/player.hpp>

int main()
{
    nana::audio::player player(STR("../Examples/audio1.wav"));
   
    player.play();  //Play the audio, it waits until the audio is finished.
}

