#pragma once
#include <memory>
#include "glm/glm.hpp"
struct SDL_Texture;
struct SDL_Renderer;
struct TTF_Font;
struct Mix_Music;
struct Mix_Chunk;

namespace engine::resource {
class TextureManager;
class AudioManager;
class FontManager;


class ResourceManager {
private:
std::unique_ptr<TextureManager> texture_manager_;
std::unique_ptr<AudioManager> audio_manager_;
std::unique_ptr<FontManager> font_manager_;


public:
ResourceManager(SDL_Renderer* renderer);
~ResourceManager();

//删除拷贝构造函数和移动
ResourceManager(const ResourceManager&) = delete;
ResourceManager& operator=(const ResourceManager&) = delete;
ResourceManager(ResourceManager&&) = delete;
ResourceManager& operator=(ResourceManager&&) = delete;

//texture
SDL_Texture* GetTexture(const char* path);
SDL_Texture* loadTexture(const char* path);
glm::vec2 getTextureSize(const char* path);
void unloadTexture(const char* path);
void clearTexture();

//audio
Mix_Music* GetMusic(const char* path);
Mix_Music* loadMusic(const char* path);
void unloadMusic(const char* path);
void clearMusic();

Mix_Chunk* GetSound(const char* path);
Mix_Chunk* loadSound(const char* path);
void unloadSound(const char* path);
void clearSound();

//font
TTF_Font* GetFont(const char* path, int size);
TTF_Font* loadFont(const char* path, int size);
void unloadFont(const char* path, int size);
void clearFont();


void resourceClear();
};
}