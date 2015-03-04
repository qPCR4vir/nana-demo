#include <nana/audio/player.hpp>

int main()
{
    nana::audio::player player(STR("./audio.wav"));
   
    player.play();  //Play the audio, it waits until the audio is finished.
}

