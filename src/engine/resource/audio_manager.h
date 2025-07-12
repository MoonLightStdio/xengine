#pragma once
#include <unordered_map>
#include <string>
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <glm/glm.hpp>

namespace engine::resource {

class AudioManager {
    friend class ResourceManager;
private:
struct SDLMixerChunkDeleter {
    void operator()(Mix_Chunk* ptr) const {
        if(ptr)
            Mix_FreeChunk(ptr);
    }
};
struct SDLMixerMusicDeleter {
    void operator()(Mix_Music* ptr) const {
        if(ptr)
            Mix_FreeMusic(ptr);
    }
};
std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixerChunkDeleter>> chunks_;
std::unordered_map<std::string, std::unique_ptr<Mix_Music, SDLMixerMusicDeleter>> musics_;

public:

    AudioManager();
    ~AudioManager();
    //删除拷贝构造函数和移动拷贝函数
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;
private:
Mix_Chunk* loadChunk(const std::string& path);
Mix_Chunk* getChunk(const std::string& path);
void unloadChunk(const std::string& path);
void clearChunks();
Mix_Music* loadMusic(const std::string& path);
Mix_Music* getMusic(const std::string& path);
void unloadMusic(const std::string& path);
void clearMusics();
void clearMixs();
};
}