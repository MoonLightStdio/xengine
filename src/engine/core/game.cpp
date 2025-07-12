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

namespace engine::core{
    Game::Game()
    {
    }

    Game::~Game()
    {
    }

bool Game::init(){
    spdlog::trace("初始化SDL...");
    if(!initSDL()) return false;
    if(!initTime()) return false;
    if(!initResourceManager()) return false;
    if(!initCamera()) return false;
    if(!initRenderer()) return false;



    spdlog::trace("初始化SDL成功...");
    return true;
}

void Game::run()
{
    FPS_=144;
    is_running=true;
    if(!init()){
        spdlog::warn("SDL init()失败...");
    }
    time_->setFps(FPS_);

    resourceManagerTest();
    while(is_running){
        float dt=time_->tick();
        update(dt);
        handleEvent();
        renderer();
    }

}

void Game::update(float dt_){

}
void Game::handleEvent(){

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if( event.type == SDL_EVENT_QUIT){
            is_running=false;
        }
    }
}
void Game::renderer(){
    SDL_RenderClear(render_);
    rendererTest();
    SDL_RenderPresent(render_);
}
bool Game::initSDL()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        spdlog::error("SDL 初始化失败! SDL错误: {}", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("SunnyLand", 1280, 720, SDL_WINDOW_RESIZABLE);
    if (window_ == nullptr) {
        spdlog::error("无法创建窗口! SDL错误: {}", SDL_GetError());
        return false;
    }

    render_ = SDL_CreateRenderer(window_, nullptr);
    if (render_ == nullptr) {
        spdlog::error("无法创建渲染器! SDL错误: {}", SDL_GetError());
        return false;
    }

    // 设置逻辑分辨率
    SDL_SetRenderLogicalPresentation(render_, 640, 360, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    spdlog::trace("SDL 初始化成功。");
    return true;
}
bool Game::initTime(){
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
    engine::render::Sprite sprite_world("assets/textures/Actors/frog.png");
    renderer_->drawSprite(camera_.get(), &sprite_world, glm::vec2(200, 200), glm::vec2(1.0f, 1.0f), 0.0f);
}
}