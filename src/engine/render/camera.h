#pragma once
#include "glm/glm.hpp"
#include "../utils/math.h"
#include "optional"
namespace engine::render {
class Camera {
private:
glm::vec2 position_= glm::vec2(0, 0);
glm::vec2 viewpoint_size_= glm::vec2(0, 0);
std::optional<engine::utils::Rect> limit_bounds_;
public:
    Camera(glm::vec2 viewpoint_size,glm::vec2 position={0.0f,0.0f}, std::optional<engine::utils::Rect> limit_bounds=std::nullopt);

    glm::vec2 getPosition() const;
    void setPosition(glm::vec2 position) ;
    glm::vec2 getViewpointSize() const;
    void setViewpointSize(glm::vec2 viewpoint_size);
    std::optional<engine::utils::Rect> getLimitBounds() const;
    void setLimitBounds(std::optional<engine::utils::Rect> limit_bounds);

    glm::vec2 getWorldPosition(glm::vec2 screen_position);
    glm::vec2 getScreenPosition(glm::vec2 world_position);
    glm::vec2 getParallaxPosition(glm::vec2 world_position,glm::vec2 scroll_factor);
    


    //删除拷贝构造函数和移动拷贝构造函数
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    
};
}