#include <nana/audio/player.hpp>

int main()
{
#ifdef NANA_ENABLE_AUDIO

    nana::audio::player player("../Examples/audio1.wav");
    player.play();  //Play the audio, it waits until the audio is finished.

#endif	//NANA_ENABLE_AUDIO

}

