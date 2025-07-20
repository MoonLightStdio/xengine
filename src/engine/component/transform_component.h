#pragma once
#include "./component.h"
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"
namespace engine::component {
class TransformComponent : public Component {
    friend class engine::object::GameObject;
public:
glm::vec2 position_ ={0.0f,0.0f};
glm::vec2 scale_ = {1.0f,1.0f};
float rotation_ =0.0f;

TransformComponent(glm::vec2 position={0.0f,0.0f}, glm::vec2 scale= {1.0f,1.0f}, float rotation=0.0f)
:position_(position),scale_(scale),rotation_(rotation) {
spdlog::info("TransformComponent created");

}

//禁止拷贝构造函数
TransformComponent(const TransformComponent&) = delete;
//禁止拷贝赋值运算符
TransformComponent& operator=(const TransformComponent&) = delete;
//禁止移动
TransformComponent(TransformComponent&&) = delete;
//禁止移动赋值运算符
TransformComponent& operator=(TransformComponent&&) = delete;

// getters and setters
glm::vec2 getPosition() const { return position_; }
void setPosition(glm::vec2 position) { position_ = position; }

glm::vec2 getScale() const { return scale_; }
void setScale(glm::vec2 scale);

float getRotation() const { return rotation_; }
void setRotation(float rotation) { rotation_ = rotation; }

private:
    /// @brief 覆盖纯虚函数
    /// @param  
    /// @param  
    void update(float,engine::core::Context&) override {} 


};
}