#include "../SDL2/include/SDL2/SDL_mixer.h"
#include "vector"

class Audio{
public:
    std::vector<Mix_Chunk*> sounds;
    std::vector<Mix_Music*> music;

    Audio();
    ~Audio();

    int addSong(const char* src);
    int addSound(const char* src);

    void playSound(int ind);
    void playMusic(int ind, bool loop = false);
};