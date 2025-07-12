#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL_render.h>
#include <memory>
#include <glm/glm.hpp>

/*
* 构建想法： 使用unordered_map来管理纹理，key为纹理名称，value为纹理指针,定义删除函数
*
*/
namespace engine::resource {
class TextureManager {
friend class ResourceManager;
private:
struct SDLTextureDeleter
{
    void operator()(SDL_Texture* texture) const
    {
        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }
    }
};

SDL_Renderer* renderer_=nullptr;
std::unordered_map<std::string, std::unique_ptr<SDL_Texture,SDLTextureDeleter>> textures_;


public:
TextureManager(SDL_Renderer* renderer);
~TextureManager();

// 删除拷贝构造函数和移动拷贝构造函数
TextureManager(const TextureManager&) = delete;
TextureManager& operator=(const TextureManager&) = delete;
TextureManager(TextureManager&&) = delete;
TextureManager& operator=(TextureManager&&) = delete;

private:
SDL_Texture* loadTexture(const std::string& path);
SDL_Texture* getTexture(const std::string& path);
glm::vec2 getTextureSize(const std::string& path);
void unloadTexture(const std::string& path);
void clearTextures();
};
}