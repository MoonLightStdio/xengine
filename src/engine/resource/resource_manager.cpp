#include "resource_manager.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "audio_manager.h"
#include "texture_manager.h"
#include "font_manager.h"
#include "spdlog/spdlog.h"

namespace engine::resource
{

    ResourceManager::ResourceManager(SDL_Renderer *renderer)
    {
        texture_manager_ = std::make_unique<TextureManager>(renderer);
        audio_manager_ = std::make_unique<AudioManager>();
        font_manager_ = std::make_unique<FontManager>();
        spdlog::info("ResourceManager created");
    }

    ResourceManager::~ResourceManager(){
        clearTexture();
        clearMusic();
        clearSound();
        clearFont();
    }

    SDL_Texture *ResourceManager::GetTexture(const char *path)
    {
        return texture_manager_->getTexture(path);
    }

    SDL_Texture *ResourceManager::loadTexture(const char *path)
    {
        return texture_manager_->loadTexture(path);
    }

    glm::vec2 ResourceManager::getTextureSize(const char *path)
    {
        return texture_manager_->getTextureSize(path);
    }

    void ResourceManager::unloadTexture(const char *path)
    {
        texture_manager_->unloadTexture(path);
    }
    void ResourceManager::clearTexture()
    {
        texture_manager_->clearTextures();
    }

    Mix_Music *ResourceManager::GetMusic(const char *path)
    {
        return audio_manager_->getMusic(path);
    }

    Mix_Music *ResourceManager::loadMusic(const char *path)
    {
        return audio_manager_->loadMusic(path);
    }

    void ResourceManager::unloadMusic(const char *path)
    {
        audio_manager_->unloadMusic(path);
    }

    void ResourceManager::clearMusic()
    {
        audio_manager_->clearMusics();
    }

    Mix_Chunk *ResourceManager::GetSound(const char *path)
    {
        return audio_manager_->getChunk(path);
    }
    Mix_Chunk *ResourceManager::loadSound(const char *path)
    {
        return audio_manager_->loadChunk(path);
    }

    void ResourceManager::unloadSound(const char *path)
    {
        audio_manager_->unloadChunk(path);
    }

    void ResourceManager::clearSound()
    {
        audio_manager_->clearChunks();
    }

    TTF_Font *ResourceManager::GetFont(const char *path,int size)
    {
        return font_manager_->getFont(path,size);
    }
    TTF_Font *ResourceManager::loadFont(const char *path,int size)
    {
        return font_manager_->loadFont(path,size);
    }

    void ResourceManager::unloadFont(const char *path,int size)
    {
        font_manager_->unloadFont(path,size);
    }
    void ResourceManager::clearFont()
    {
        font_manager_->clearFonts();

    }
    void ResourceManager::resourceClear()
    {
        clearTexture();
        clearMusic();
        clearSound();
        clearFont();
    }
}