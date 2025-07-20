#pragma once

namespace engine::input {
    class InputManager;
}
namespace engine::render {
    class Camera;
    class Renderer;
}

namespace engine::resource {
    class ResourceManager;
}

namespace engine::core {
class Context final{ //没有继承类
private:
    //使用引用确保每个模块都有效，使用时不用检查是否为null
    engine::input::InputManager& input_manager_;
    engine::render::Camera& camera_;
    engine::render::Renderer& renderer_;
    engine::resource::ResourceManager& resource_manager_;


public:
Context(engine::input::InputManager& input_manager,
    engine::render::Camera& camera,
    engine::render::Renderer& renderer, 
    engine::resource::ResourceManager& resource_manager);


    //禁止拷贝构造和赋值 和移动构造和赋值
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;


    //Getters
    engine::input::InputManager& getInputManager() const {return input_manager_;}
    engine::render::Camera& getCamera() const {return camera_;}
    engine::render::Renderer& getRenderer() const {return renderer_;}
    engine::resource::ResourceManager& getResourceManager() const {return resource_manager_;}

    //


private:
};
}