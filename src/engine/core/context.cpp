#include "context.h"
#include "../input/input_manager.h"
#include "../render/camera.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "spdlog/spdlog.h"
namespace engine::core{
Context::Context(engine::input::InputManager &input_manager, 
    engine::render::Camera &camera, 
    engine::render::Renderer &renderer, 
    engine::resource::ResourceManager &resource_manager)
    : input_manager_(input_manager), 
      camera_(camera), 
      renderer_(renderer), 
      resource_manager_(resource_manager)
{
    spdlog::info("Context created,包含输入管理器、相机、渲染器和资源管理器");
}
}
