#pragma once
#include <memory>
struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource{
    class ResourceManager;
}
namespace engine::render{
    class Camera;
    class Renderer;
}
namespace engine::input{
    class InputManager;
}

namespace engine::core{
class Time;
class Config; //配置文件
class Game{
private:
float speed_=100;
int FPS_; //每秒帧率
float delay_time_; //帧延迟
SDL_Window* window_;
SDL_Renderer* render_;
bool is_running=true;


std::unique_ptr<engine::core::Time> time_;
std::unique_ptr<engine::resource::ResourceManager> resource_manager_;
std::unique_ptr<engine::render::Camera> camera_;
std::unique_ptr<engine::render::Renderer> renderer_;
std::unique_ptr<engine::core::Config> config_;
std::unique_ptr<engine::input::InputManager> input_manager_;


public:
Game();
~Game();


//删除拷贝构造和移动构造函数
Game(const Game&)=delete;
Game& operator=(const Game&)=delete;
Game(Game&&)=delete;
Game& operator=(Game&&)=delete;

[[nodiscard]] bool init();
void run();
void update(float dt_);
void handleEvent();
void renderer();
private:
[[nodiscard]] bool initSDL();
[[nodiscard]] bool initConfig();
[[nodiscard]] bool initTime();
[[nodiscard]] bool initResourceManager();
[[nodiscard]] bool initCamera();
[[nodiscard]] bool initRenderer();
[[nodiscard]] bool initInputManager();

void resourceManagerTest();
void rendererTest();
void testInputManager();
void testCamera();
void testGameObject();


};
}