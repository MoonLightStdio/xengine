#include "scene_manager.h"
#include "../scene/scene.h"
#include "spdlog/spdlog.h"
#include "../core/context.h"
namespace engine::scene{
SceneManager::SceneManager(engine::core::Context &context)
    :context_(context){
        spdlog::trace("场景管理器已创建。");
}
SceneManager::~SceneManager()
{
    spdlog::trace("场景管理器已销毁。");
    close();
}
void SceneManager::requestPushScene(std::unique_ptr<Scene> &&scene)
{
    pending_action_=PendingAction::Push;
    pending_scene_=std::move(scene);
}
void SceneManager::requestPopScene()
{
    pending_action_=PendingAction::Pop;

}
void SceneManager::requestReplaceScene(std::unique_ptr<Scene> &&scene)
{
    pending_action_=PendingAction::Replace;
    pending_scene_=std::move(scene);
}

Scene *SceneManager::getCurrentScene() const
{
    if(scene_stack_.empty()){
        return nullptr;
    }
    return scene_stack_.back().get();
}

void SceneManager::processPendingActions(){
    if(pending_action_ == PendingAction::None){
        return;
    }
    switch(pending_action_){
        case PendingAction::Push:
            pushScene(std::move(pending_scene_));
            break;
        case PendingAction::Pop:
            popScene();
            break;
        case PendingAction::Replace:
            replaceScene(std::move(pending_scene_));
            break;
        default:
            break;
    }
    pending_action_=PendingAction::None;
}
void SceneManager::pushScene(std::unique_ptr<Scene>&& scene){
    if(!scene){
        spdlog::warn("尝试将空场景压入栈");
        return;
    }
    spdlog::debug("正在压入场景 '{}'",scene->getName());

    // 初始化新场景
    if(!scene->isInitialized()){ //初始化一次
        scene->init();
    }

    // 将新场景压入堆栈
    scene_stack_.push_back(std::move(scene));
}
void SceneManager::popScene(){
    if(scene_stack_.empty()){
        spdlog::warn("尝试弹出空场景栈");
        return;
    }
    spdlog::debug("正在弹出场景 '{}'",scene_stack_.back()->getName());
    if(scene_stack_.back()){
        scene_stack_.back()->clean();

    }
    scene_stack_.pop_back();

}
void SceneManager::replaceScene(std::unique_ptr<Scene>&& scene){
    if(!scene){
        spdlog::warn("尝试将空场景替换");
        return;
    }
    spdlog::debug("正在替换场景 '{}'",scene->getName());
    while(!scene_stack_.empty()){
        if(scene_stack_.back()){
            scene_stack_.back()->clean();
        }
        scene_stack_.pop_back();
    }

    // 初始化新场景
    if(!scene->isInitialized()){ //初始化一次
        scene->init();
    }
    scene_stack_.push_back(std::move(scene));
}

void SceneManager::update(float delta_time){
    Scene* current_scene = getCurrentScene();
    if(current_scene){
        current_scene->update(delta_time);
    }
    processPendingActions();
}
void SceneManager::render(){
    for(const auto& scene: scene_stack_){
        if(scene){
            scene->render();
        }
    }
}
void SceneManager::handleInput(){
    Scene* current_scene =getCurrentScene();
    if(current_scene){
        current_scene->handleInput();
    }
}
void SceneManager::close(){
    spdlog::trace("关闭场景管理器。");
    while(!scene_stack_.empty()){
        if(scene_stack_.back()){
            spdlog::debug("正在清理场景 '{}",scene_stack_.back()->getName());
            scene_stack_.back()->clean();
        }
        scene_stack_.pop_back();
    }
}
}
