#include "renderer.h"
#include "camera.h"
#include "../resource/resource_manager.h"
#include "spdlog/spdlog.h"
#include "optional"
#include "SDL3/SDL.h"
#include "../utils/math.h"
namespace engine::render
{
    Renderer::Renderer(SDL_Renderer *sdl_render, engine::resource::ResourceManager *resource_manager)
{
    this->sdl_render_ = sdl_render;
    this->resource_manager_ = resource_manager;
}
void Renderer::drawSprite(Camera *camera, Sprite *sprite, glm::vec2 position, glm::vec2 scale, double angle)
{
    auto texture= resource_manager_->GetTexture(sprite->getTextureId().c_str());
    if(texture == nullptr){
        spdlog::error("Texture not found {}", sprite->getTextureId());
    }

    std::optional<SDL_FRect>resRec= getSpriteSrcRect(sprite);
    if(!resRec.has_value()){
        spdlog::error("Sprite {} does not have a src rect",sprite->getTextureId());
        return;
    }
    glm::vec2 screen_position= camera->getScreenPosition(position);
    SDL_FRect dst;
    dst.x = screen_position.x;
    dst.y = screen_position.y;
    dst.w = resRec.value().w * scale.x;
    dst.h = resRec.value().h * scale.y;

    SDL_RenderTextureRotated(sdl_render_, texture, &resRec.value(), &dst,angle,NULL,sprite->isFilp()? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
void Renderer::drawUISprite(Sprite *sprite, glm::vec2 position, glm::vec2 scale, double angle)
{

}
std::optional<SDL_FRect> Renderer::getSpriteSrcRect(const Sprite *sprite)
{
    if(sprite->getSrcRect().has_value()){
        return sprite->getSrcRect();
    }else{
        auto texture= resource_manager_->GetTexture(sprite->getTextureId().c_str());
        if(texture == nullptr){
            spdlog::error("Texture not found {}", sprite->getTextureId());
            return std::nullopt;
        }
        SDL_FRect rect={0.0f,0.0f,0.0f,0.0f};
        SDL_GetTextureSize(texture, &rect.w, &rect.h);
        return rect;
    }
}
void Renderer::present()
{
    SDL_RenderClear(sdl_render_);
    SDL_RenderPresent(sdl_render_);
}
}
