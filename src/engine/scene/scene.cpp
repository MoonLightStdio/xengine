#include "scene.h"
#include "../core/context.h"
#include "../scene/scene_manager.h"
#include "../resource/resource_manager.h"
#include "spdlog/spdlog.h"
#include "../object/game_object.h"
#include "scene_manager.h"
namespace engine::scene{
Scene::Scene(std::string name, engine::core::Context &context, engine::scene::SceneManager &scene_manager)
: scene_name_(name), context_(context), scene_manager_(scene_manager),is_initialized_(false)
{
    spdlog::info("Scene {} created", name);
}

Scene::~Scene() = default;

    void Scene::init(){
        is_initialized_ = true;
        spdlog::trace("场景 {} 初始化完成", scene_name_);
    }                        
    void Scene::update(float delta_time){
        if(!is_initialized_) return;

        for(auto it = game_objects_.begin(); it != game_objects_.end();){
            if(*it && !(*it)->isNeedRemove()){
                (*it)->update(delta_time,context_);
                ++it;
            }else{
                if(*it){
                    (*it)->clean();
                }
                it=game_objects_.erase(it); //删除需要移除的对象
            }
        }
        processPendingAdditions();  //处理待添加的游戏对象

    }   
    void Scene::render(){
        if(!is_initialized_) return;
        for(const auto& obj : game_objects_){
            if(obj) obj->render(context_);
        }
    }                   
    void Scene::handleInput(){
        if(!is_initialized_) return;
        for(auto it=game_objects_.begin();it!=game_objects_.end();){
            if((*it)&&!(*it)->isNeedRemove()){
                (*it)->handleInput(context_);
                ++it;
            }else{
                if(*it) (*it)->clean();
                it = game_objects_.erase(it);
            }
        }
    }

void Scene::addGameObject(std::unique_ptr<engine::object::GameObject> &&game_object)
    {
        //传参game_object 为右值
        if(game_object) game_objects_.push_back(std::move(game_object));
        else spdlog::warn("尝试向场景 {} 添加空的游戏对象",scene_name_);
    }
    void Scene::safeAddGameObject(std::unique_ptr<engine::object::GameObject> &&game_object)
    {
        if(game_object) pending_additions_.push_back(std::move(game_object));
        else spdlog::warn("尝试向场景 {} 添加空的游戏对象",scene_name_);
    }
    void Scene::removeGameObject(engine::object::GameObject *game_object_ptr)
    {
        if(!game_object_ptr){
            spdlog::warn("尝试从场景{},移除空的游戏对象",scene_name_);
            return;
        }
        auto it =std::remove_if(game_objects_.begin(),game_objects_.end(),[game_object_ptr](const std::unique_ptr<engine::object::GameObject>& obj){
            return obj.get() == game_object_ptr;
        });

        if(it!=game_objects_.end()){
        (*it)->clean();
        game_objects_.erase(it,game_objects_.end());
        spdlog::trace("从场景 {} 移除游戏对象 {}",scene_name_,game_object_ptr->getName());
        }else{
            spdlog::warn("尝试从场景 {} 移除不存在的游戏对象 {}",scene_name_,game_object_ptr->getName());
        }
    }
    void Scene::safeRemoveGameObject(engine::object::GameObject *game_object_ptr)
    {
        game_object_ptr->setNeedRemove(true);
    }
    engine::object::GameObject *Scene::findGameObjectByName(const std::string &name) const
    {
        for(const auto& obj:game_objects_){
            if(obj&&obj->getName()==name) return obj.get();
        }
        return nullptr;
    }
    void Scene::processPendingAdditions()
    {
        for(auto &game_objects:pending_additions_){
            addGameObject(std::move(game_objects));
        }
        pending_additions_.clear();
    }
    void Scene::clean()
    {
        if(!is_initialized_) return;
        for(const auto& obj:game_objects_){
            if(obj) obj->clean();
        }
        game_objects_.clear();
        is_initialized_ = false;
        spdlog::trace("场景 {} 清理完成",scene_name_);
    }
}
