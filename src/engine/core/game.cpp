#include "game.h"

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "../resource/resource_manager.h"

#include "../render/camera.h"
#include "../render/renderer.h"
#include "../utils/math.h"
#include "time.h"
#include "config.h"
#include "../input/input_manager.h"
#include "../object/game_object.h"
#include "../component/component.h"
#include "context.h"
#include "../component/sprite_component.h"
#include "../component/transform_component.h"
#include "../scene/scene_manager.h"
#include "../../game/scene/game_scene.h"
namespace engine::core{

engine::object::GameObject game_object("Test_game_object");
    Game::Game()
    {
    }

    Game::~Game()
    {
    }

bool Game::init(){
    spdlog::trace("初始化SDL...");
    if(!initConfig()) return false;
    if(!initSDL()) return false;
    if(!initTime()) return false;
    if(!initResourceManager()) return false;
    if(!initCamera()) return false;
    if(!initRenderer()) return false;
    if(!initInputManager()) return false;
    if(!initContext()) return false;
    if(!initSceneManager()) return false;

    // 创建第一个场景并压入栈
    auto scene = std::make_unique<game::scene::GameScene>("GameScene", *context_, *scene_manager_);
    scene_manager_->requestPushScene(std::move(scene));

    is_running = true;
    spdlog::trace("GameApp 初始化成功。");
    return true;
}

void Game::run()
{
    if(!init()){
        spdlog::warn("SDL init()失败...");
    }
    FPS_=config_->target_fps_;
    is_running=true;

    time_->setFps(FPS_);


    while(is_running){
        float dt=time_->tick();
       // spdlog::info("dt:{},",dt);
       input_manager_->update();
        handleEvent();
        update(dt);
        renderer();
    }

}

void Game::update(float dt_)
{
    scene_manager_->update(dt_);
}
void Game::handleEvent(){
    if (input_manager_->shouldQuit()) {
        spdlog::trace("GameApp 收到来自 InputManager 的退出请求。");
        is_running = false;
        return;
    }

    scene_manager_->handleInput();

}
void Game::renderer(){
    SDL_RenderClear(render_);
    scene_manager_->render();
    SDL_RenderPresent(render_);
}
bool Game::initSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        spdlog::error("SDL 初始化失败! SDL错误: {}", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(config_->window_title_.c_str(), config_->window_width_, config_->window_height_,config_->window_resizable_? SDL_WINDOW_RESIZABLE:NULL);
    if (window_ == nullptr) {
        spdlog::error("无法创建窗口! SDL错误: {}", SDL_GetError());
        return false;
    }

    render_ = SDL_CreateRenderer(window_, nullptr);
    if (render_ == nullptr) {
        spdlog::error("无法创建渲染器! SDL错误: {}", SDL_GetError());
        return false;
    }
// 设置 VSync (注意: VSync 开启时，驱动程序会尝试将帧率限制到显示器刷新率，有可能会覆盖我们手动设置的 target_fps)
    int vsync_mode = config_->vsync_enabled_ ? SDL_RENDERER_VSYNC_ADAPTIVE : SDL_RENDERER_VSYNC_DISABLED;
    SDL_SetRenderVSync(render_, vsync_mode);
    spdlog::trace("VSync 设置为: {}", config_->vsync_enabled_ ? "Enabled" : "Disabled");
    // 设置逻辑分辨率
    SDL_SetRenderLogicalPresentation(render_,  config_->window_width_/2, config_->window_height_/2,SDL_LOGICAL_PRESENTATION_LETTERBOX);
    spdlog::trace("SDL 初始化成功。");
    return true;
}
bool Game::initConfig()
{
    try{
        config_=std::make_unique<Config>("assets/config.json");
    }catch(std::exception& e){
        spdlog::error("初始化配置失败: {}",e.what());
        return false;
    }
    spdlog::trace("config初始化配置成功。");
    return true;
}
bool Game::initTime()
{
    try{
    time_=std::make_unique<Time>();
    }catch(std::exception& e){
        spdlog::error("初始化时间失败: {}",e.what());
        return false;
    }
    return true;
}
bool Game::initResourceManager()
{
    try{
        resource_manager_ = std::make_unique<engine::resource::ResourceManager>(render_);
    }catch(std::exception& e){
        spdlog::error("初始化资源管理器失败: {}",e.what());
        return false;
    }
    return true;
}
bool Game::initCamera()
{
    try{
        glm::vec2 screen_size(640.0f, 360.0f);
        camera_ = std::make_unique<engine::render::Camera>(screen_size);
    }catch(std::exception& e){
        spdlog::error("初始化相机失败: {}",e.what());
        return false;
    }
    spdlog::trace("初始化相机成功。");
    return true;
}
bool Game::initRenderer()
{
    try{
        renderer_ = std::make_unique<engine::render::Renderer>(render_,resource_manager_.get());
    }catch(std::exception& e){
        spdlog::error("初始化渲染器失败: {}",e.what());
        return false;
    }
    spdlog::trace("初始化渲染器成功。");
    return true;
}
bool Game::initInputManager()
{
    try {
        input_manager_ = std::make_unique<engine::input::InputManager>(render_, config_.get());
    } catch (const std::exception& e) {
        spdlog::error("初始化输入管理器失败: {}", e.what());
        return false;
    }
    spdlog::trace("输入管理器初始化成功。");
    return true;
}
bool Game::initContext()
{
    try{
        context_ = std::make_unique<engine::core::Context>(*input_manager_,  *camera_, *renderer_,*resource_manager_);
    }catch(const std::exception& e){
        spdlog::error("初始化上下文失败: {}",e.what());
        return false;

    }
    return true;
}
bool Game::initSceneManager()
{
    try{
        scene_manager_ = std::make_unique<engine::scene::SceneManager>(*context_);
    }catch(const std::exception& e){
        spdlog::error("初始化场景管理器失败: {}",e.what());
        return false;
    }
    return true;
}
/*
void Game::testCamera()
{
    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_UP]) {
        camera_->move(glm::vec2(0, -1));   
        spdlog::info("camera move up, camera_.position: {},{}",camera_->getPosition().x,camera_->getPosition().y);
    }
    if (key_state[SDL_SCANCODE_DOWN]) camera_->move(glm::vec2(0, 1));
    if (key_state[SDL_SCANCODE_LEFT]) camera_->move(glm::vec2(-1, 0));
    if (key_state[SDL_SCANCODE_RIGHT]) camera_->move(glm::vec2(1, 0));
}
void Game::testGameObject()
{
    game_object.addComponent<engine::component::TransformComponent>(glm::vec2(100, 100));
    game_object.addComponent<engine::component::SpriteComponent>("assets/textures/Props/big-crate.png", *resource_manager_, engine::utils::Alignment::CENTER);
    game_object.getComponent<engine::component::TransformComponent>()->setScale(glm::vec2(2.0f,2.0f));
    game_object.getComponent<engine::component::TransformComponent>()->setRotation(45.0f);
}
void Game::testInputManager()
{
    std::vector<std::string> actions = {
        "move_up",
        "move_down",
        "move_left",
        "move_right",
        "jump",
        "attack",
        "pause",
        "MouseLeftClick",
        "MouseRightClick"
    };
    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_ESCAPE]) is_running = false;
    for (const auto& action : actions) {
        if (input_manager_->isActionPressed(action)) {
           // spdlog::info(" {} 按下 ", action);
        }
        if (input_manager_->isActionReleased(action)) {
          //  spdlog::info(" {} 抬起 ", action);
        }
        if (input_manager_->isActionDown(action)) {
          //  spdlog::info(" {} 按下中 ", action);
        }
    }
}
void Game::resourceManagerTest()
{
    resource_manager_->GetTexture("assets/textures/Actors/eagle-attack.png");
    resource_manager_->GetFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
    resource_manager_->GetSound("assets/audio/button_click.wav");

    resource_manager_->unloadTexture("assets/textures/Actors/eagle-attack.png");
    resource_manager_->unloadFont("assets/fonts/VonwaonBitmap-16px.ttf", 16);
    resource_manager_->unloadSound("assets/audio/button_click.wav");
}
void Game::rendererTest()
{
    static float rotation = 0.0f;
    rotation += 0.01f;
    engine::render::Sprite sprite_world("assets/textures/Actors/frog.png");
    engine::render::Sprite sprite_ui("assets/textures/UI/credits-text.png");
    engine::render::Sprite sprite_parallax("assets/textures/Layers/back.png");

    renderer_->drawParallax( camera_.get(),&sprite_parallax, glm::vec2(100,100),glm::vec2(1.0f, 1.0f),glm::vec2(0.5f, 0.5f));
    renderer_->drawSprite(camera_.get(), &sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), rotation);
    renderer_->drawUISprite(&sprite_ui, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f);
}
*/
}