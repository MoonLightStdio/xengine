#include "camera.h"


namespace engine::render{
Camera::Camera( glm::vec2 viewpoint_size, glm::vec2 position,std::optional<engine::utils::Rect> limit_bounds)
{
    this->position_ = position;
    this->viewpoint_size_ = viewpoint_size;
    this->limit_bounds_ = limit_bounds;
}

glm::vec2 Camera::getPosition() const
{
    return position_;
}

glm::vec2 Camera::getViewpointSize() const
{
    return viewpoint_size_;
}

std::optional<engine::utils::Rect> Camera::getLimitBounds() const
{
    return limit_bounds_;
}
void Camera::setPosition(glm::vec2 position)
{
    this->position_ = position;
}
void Camera::setViewpointSize(glm::vec2 viewpoint_size)
{
    this->viewpoint_size_ = viewpoint_size;
}
void Camera::setLimitBounds(std::optional<engine::utils::Rect> limit_bounds)
{
    this->limit_bounds_ = limit_bounds;
}

glm::vec2 Camera::getWorldPosition(glm::vec2 screen_position)
{
    return screen_position+position_;
}
glm::vec2 Camera::getScreenPosition(glm::vec2 world_position)
{
    return world_position-position_;
}
glm::vec2 Camera::getParallaxPosition(glm::vec2 world_position, glm::vec2 scroll_factor)
{
    return world_position-position_*scroll_factor;
}
}
