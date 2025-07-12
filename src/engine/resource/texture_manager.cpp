#include "texture_manager.h"
#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"
#include "SDL3_image/SDL_image.h"
namespace engine::resource{

TextureManager::TextureManager(SDL_Renderer* renderer) {
    this->renderer_ = renderer;
    if(renderer==nullptr){
        spdlog::error("renderer is null");
    }
    spdlog::info("TextureManager created");
    
}

TextureManager::~TextureManager() {
    clearTextures();
    spdlog::info("TextureManager destroyed");
}


void TextureManager::clearTextures() {
    
}

SDL_Texture* TextureManager::loadTexture(const std::string& filePath) {
    auto it=textures_.find(filePath);
    if(it!=textures_.end()){
        return it->second.get();
    }
    SDL_Texture* texture = IMG_LoadTexture(renderer_, filePath.c_str());
    if(texture==nullptr){
        spdlog::warn("Failed to load texture: {}",filePath);
        return nullptr;
    }
    textures_.emplace(filePath,std::unique_ptr<SDL_Texture,SDLTextureDeleter>(texture));
    return texture;
}
SDL_Texture* TextureManager::getTexture(const std::string& filePath) {
    auto it=textures_.find(filePath);
    if(it!=textures_.end()){
        return it->second.get();
    }
    return loadTexture(filePath);
}
void TextureManager::unloadTexture(const std::string& filePath) {
    auto it=textures_.find(filePath);
    if(it!=textures_.end()){
        textures_.erase(it);
        spdlog::info("Texture unloaded: {}",filePath);
    }
}

glm::vec2 TextureManager::getTextureSize(const std::string& filePath) {
    auto it=textures_.find(filePath);
    if(it!=textures_.end()){
        SDL_Texture* texture = it->second.get();
        glm::vec2 textureSize_;
        SDL_GetTextureSize(texture,&textureSize_.x,&textureSize_.y);
        return textureSize_;
    }
    return glm::vec2(0,0);
}

}