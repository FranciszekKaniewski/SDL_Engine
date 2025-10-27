#include "Headers/Audio.hpp"
#include "iostream"

Audio::Audio() {}

int Audio::addSong(const char *src) {
    Mix_Music* tmp = Mix_LoadMUS(src);
    music.push_back(tmp);
    if(tmp == NULL){
        std::cerr << "Error loading music "<< src << ": " << Mix_GetError() << std::endl;
    }
    return music.size()-1;
}

int Audio::addSound(const char *src) {
    Mix_Chunk* tmp = Mix_LoadWAV(src);
    sounds.push_back(tmp);
    if(tmp == NULL){
        std::cerr << "Error loading sound "<< src << ": " << Mix_GetError() << std::endl;
    }
    return sounds.size()-1;
}

Audio::~Audio() {
    for(Mix_Music* m : music){
        if(m) Mix_FreeMusic(m);
    }
    music.clear();

    for(Mix_Chunk* s : sounds){
        if(s) Mix_FreeChunk(s);
    }
    sounds.clear();

    Mix_CloseAudio();
    Mix_Quit();
}

void Audio::playMusic(int ind, bool loop) {
    if (!music[ind]) {
        std::cerr << "Music Error: "<< ind << ": " << Mix_GetError() << std::endl;
        return;
    }
    Mix_PlayMusic(music[ind],loop ? -1 : 0);
}

void Audio::playSound(int ind) {
    if (!sounds[ind]) {
        std::cerr << "Mix_LoadMUS Error: " << ind << ": " << Mix_GetError() << std::endl;
        return;
    }

    Mix_Volume(-1, 1000);
    Mix_PlayChannel(-1,sounds[ind], 0);
}