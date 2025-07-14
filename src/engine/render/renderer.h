#pragma once
#include "glm/glm.hpp"
#include "sprite.h"
#include "optional"
struct SDL_Renderer;
namespace engine::resource{
    class ResourceManager;
}
namespace engine::render{
class Camera;
class Renderer{
private:
SDL_Renderer *sdl_render_;
engine::resource::ResourceManager *resource_manager_;

public:
    Renderer(SDL_Renderer *sdl_render, engine::resource::ResourceManager *resource_manager);
    void drawSprite(Camera *camera, Sprite *sprite, glm::vec2 position, glm::vec2 scale={1.0f,1.0f} , double angle=0.0f);
    void drawUISprite(Sprite *sprite, glm::vec2 position, glm::vec2 scale={1.0f,1.0f} , double angle=0.0f);
    void drawParallax(Camera *camera, Sprite *sprite, glm::vec2 position,glm::bvec2 repeat,glm::vec2 scroll_factor, glm::vec2 scale={1.0f,1.0f});
    void present();

    SDL_Renderer *getSDLRenderer() const;
    std::optional<SDL_FRect>getSpriteSrcRect(const Sprite *sprite);
};
}