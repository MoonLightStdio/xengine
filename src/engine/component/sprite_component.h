#pragma once 
#include "../render/sprite.h"
#include "component.h"
#include "../utils/alignment.h"

#include <string>
#include <optional>
#include "SDL3/SDL_rect.h"
#include "glm/vec2.hpp"


namespace engine::core {
    class Context;
}

namespace engine::resource {
    class ResourceManager;
}

namespace engine::component {
    /// @brief 存储位置，位移变化的组件
    class TransformComponent;

class SpriteComponent final: public Component {
    friend class engine::object::GameObject;   //友元不能继承，必须单独添加

private:
    engine::resource::ResourceManager* resource_manager_=nullptr;
    TransformComponent* transform_ = nullptr;

    engine::render::Sprite sprite_;
    engine::utils::Alignment alignment_ = engine::utils::Alignment::NONE; //对齐方式
    glm::vec2 sprite_size_ = {0.0f,0.0f};
    glm::vec2 offset_={0.0f,0.0f};
    bool is_hidden_ = false;


public:
    SpriteComponent(const std::string& texture_id,
        engine::resource::ResourceManager& resource_manager, 
        engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
    std::optional<SDL_FRect> source_rect_opt = std::nullopt,
bool is_flipped= false);
    ~SpriteComponent() override = default;  

    //禁止拷贝构造和赋值和 移动构造和赋值
    SpriteComponent(const SpriteComponent&) = delete;
    SpriteComponent& operator=(const SpriteComponent&) = delete;
    SpriteComponent(SpriteComponent&&) = delete;
    SpriteComponent& operator=(SpriteComponent&&) = delete;

    void updateOffset();  //更新偏移量,根据当前的alignment_和sprite_size_来计算

    //Getters
    const engine::render::Sprite& getSprite() const { return sprite_; }
    const std::string& getTextureId() const { return sprite_.getTextureId(); }
    bool isHidden() const { return is_hidden_; }
    const glm::vec2& getSpriteSize() const { return sprite_size_; }
    const glm::vec2& getOffset() const { return offset_; }
    engine::utils::Alignment getAlignment() const { return alignment_; }

    //Setters
    void setSpriteById(const std::string& texture_id,const std::optional<SDL_FRect>& source_rect_opt=std::nullopt);
    void setFipped(bool is_flipped) { sprite_.setFilp(is_flipped); }
    void setAlignment(engine::utils::Alignment alignment);
    void setHidden(bool is_hidden) { is_hidden_ = is_hidden; }
    void setSourceRect(const SDL_FRect& source_rect_opt);

private:
    /// @brief 更新精灵图大小
    void updateSpriteSize();

    void init() override;
    void update(float, engine::core::Context&) override{} //先为空
    void render(engine::core::Context&) override;


};

}
