#include "renderer.h"
#include "camera.h"
#include "../resource/resource_manager.h"
#include "spdlog/spdlog.h"
#include "optional"
#include "SDL3/SDL.h"
#include "../utils/math.h"
#include <iostream>
#include <algorithm> 
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
    auto texture= resource_manager_->GetTexture(sprite->getTextureId().c_str());
    if(texture == nullptr){
        spdlog::error("Texture not found {}", sprite->getTextureId());
    }

    std::optional<SDL_FRect>resRec= getSpriteSrcRect(sprite);
    if(!resRec.has_value()){
        spdlog::error("Sprite {} does not have a src rect",sprite->getTextureId());
        return;
    }
    SDL_FRect dst;
    dst.x = position.x;
    dst.y = position.y;
    dst.w = resRec.value().w * scale.x;
    dst.h = resRec.value().h * scale.y;

    SDL_RenderTextureRotated(sdl_render_, texture, &resRec.value(), &dst,angle,NULL,sprite->isFilp()? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

}
void Renderer::drawParallax(Camera *camera, Sprite *sprite, glm::vec2 position, glm::bvec2 repeat,glm::vec2 scroll_factor, glm::vec2 scale)
{
    auto texture=resource_manager_->GetTexture(sprite->getTextureId().c_str());
    if(texture==nullptr){
        spdlog::error("parallax {} not found!",sprite->getTextureId());
        return;
    }
    std::optional<SDL_FRect> rect=getSpriteSrcRect(sprite);
    if(!rect.has_value()){
        spdlog::error("parallax has no FRect!");
        return;
    }
    glm::vec2 parallax_position=camera->getParallaxPosition(position,scroll_factor);
    //精灵图大小
    float scroll_x=rect.value().w *scale.x;
    float scroll_y=rect.value().h *scale.y;
    glm::vec2 start,stop;
    if(repeat.x==true){
        start.x=glm::mod(parallax_position.x,scroll_x)-scroll_x;
        stop.x=camera->getViewpointSize().x;
    }else{
        start.x=parallax_position.x;
        stop.x=std::min(parallax_position.x+scroll_x,camera->getViewpointSize().x);
    }
    if(repeat.y==true){
        start.y=glm::mod(parallax_position.y,scroll_y)-scroll_y;
        stop.y=camera->getViewpointSize().y;
    }else{
        start.y=parallax_position.y;
        stop.y=std::min(parallax_position.y+scroll_y,camera->getViewpointSize().y);
    }
    for(float x=start.x;x<stop.x;x+=scroll_x){
        
        for(float y=start.y;y<stop.y;y+=scroll_y){
            SDL_FRect dst;
            dst.x=x;
            dst.y=y;
            dst.w=rect.value().w *scale.x;
            dst.h=rect.value().h *scale.y;
            SDL_RenderTextureRotated(sdl_render_,texture,&rect.value(),&dst,0,NULL,sprite->isFilp()?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
        }
    }
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
