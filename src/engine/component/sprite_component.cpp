#include "sprite_component.h"
#include "transform_component.h"
#include "../object/game_object.h"
#include "../core/context.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "../render/camera.h"
#include <stdexcept>          // 用于 std::runtime_error
#include <spdlog/spdlog.h>


namespace engine::component
{
    SpriteComponent::SpriteComponent(const std::string &texture_id, 
        engine::resource::ResourceManager& resource_manager, 
        engine::utils::Alignment alignment, 
        std::optional<SDL_FRect> source_rect_opt, 
        bool is_flipped
    ):resource_manager_(&resource_manager), sprite_(texture_id,source_rect_opt, is_flipped), alignment_(alignment)
{
    if(!resource_manager_){
        spdlog::critical("SpriteComponent: resource_manager_ is nullptr");
        //不能再游戏的主循环中抛出异常，会严重影响游戏性能

    }
    spdlog::trace("创建 SpriteComponent，纹理为：{}",texture_id);
}
void SpriteComponent::updateOffset()
{
    if(sprite_size_.x<=0||sprite_size_.y<=0){
        offset_= {0.0f,0.0f};
        return ;
    }
    auto scale = transform_->getScale();
    switch(alignment_){
         case engine::utils::Alignment::TOP_LEFT:      offset_ = glm::vec2{0.0f, 0.0f} * scale; break;
        case engine::utils::Alignment::TOP_CENTER:    offset_ = glm::vec2{-sprite_size_.x / 2.0f, 0.0f} * scale; break;
        case engine::utils::Alignment::TOP_RIGHT:     offset_ = glm::vec2{-sprite_size_.x, 0.0f} * scale; break;
        case engine::utils::Alignment::CENTER_LEFT:   offset_ = glm::vec2{0.0f, -sprite_size_.y / 2.0f} * scale; break;
        case engine::utils::Alignment::CENTER:        offset_ = glm::vec2{-sprite_size_.x / 2.0f, -sprite_size_.y / 2.0f} * scale; break;
        case engine::utils::Alignment::CENTER_RIGHT:  offset_ = glm::vec2{-sprite_size_.x, -sprite_size_.y / 2.0f} * scale; break;
        case engine::utils::Alignment::BOTTOM_LEFT:   offset_ = glm::vec2{0.0f, -sprite_size_.y} * scale; break;
        case engine::utils::Alignment::BOTTOM_CENTER: offset_ = glm::vec2{-sprite_size_.x / 2.0f, -sprite_size_.y} * scale; break;
        case engine::utils::Alignment::BOTTOM_RIGHT:  offset_ = glm::vec2{-sprite_size_.x, -sprite_size_.y} * scale; break;
        case engine::utils::Alignment::NONE:
        default:          break;         
    }
}
void SpriteComponent::setSpriteById(const std::string &texture_id, const std::optional<SDL_FRect> &source_rect_opt)
{
    sprite_.setTextureId(texture_id);
    sprite_.setResRect(source_rect_opt);

    updateSpriteSize();
    updateOffset();
}
void SpriteComponent::setAlignment(engine::utils::Alignment alignment)
{
    alignment_ = alignment;
    updateOffset();
}
void SpriteComponent::setSourceRect(const SDL_FRect &source_rect_opt)
{
    sprite_.setResRect(source_rect_opt);
    updateSpriteSize();
    updateOffset();
}
void SpriteComponent::updateSpriteSize()
{
    if(!resource_manager_){
        spdlog::error("ResourceManager is nullptr");
        return;
    }
    if(sprite_.getSrcRect().has_value()){
        const auto& src_rect = sprite_.getSrcRect().value();
        sprite_size_ = {src_rect.w, src_rect.h};
    }else{
        sprite_size_ = resource_manager_->getTextureSize(sprite_.getTextureId().c_str());
    }
}
void SpriteComponent::init()
{
    if(!owner_){
        spdlog::error("SpriteComponent: owner_ is nullptr,没有拥有的游戏对象");
        return ;
    }
    transform_ = owner_->getComponent<TransformComponent>();
    if(!transform_){
        spdlog::error("SpriteComponent:{} transform_ is nullptr,没有拥有的transform组件",owner_->getName());
        return ;
    }
    updateSpriteSize();
    updateOffset();

}
void SpriteComponent::render(engine::core::Context & context)
{
    if(is_hidden_||!transform_||!resource_manager_){
        return ;
    }

    //获取变化信息 （考虑偏移量）
    const glm::vec2& pos = transform_->getPosition() + offset_;
    const glm::vec2& scale = transform_->getScale();
    float rotation_degrees = transform_->getRotation();

    context.getRenderer().drawSprite(&context.getCamera(),&sprite_, pos, scale, rotation_degrees);
}
}