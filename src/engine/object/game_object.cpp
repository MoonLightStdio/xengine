#include "game_object.h"
#include "../core/context.h"
namespace engine::object {

    GameObject::GameObject(const std::string& name,const std::string& tag){
        name_ = name;
        tag_ = tag;
    spdlog::trace("GameObject created: {} {}", name_, tag_);

    }
    void GameObject::update(float delta_time,engine::core::Context& context){
        for(auto& component : components_){
            component.second->update(delta_time,context);
        }
    }
    void GameObject::render(engine::core::Context& context){
        for(auto& component : components_){
            component.second->render(context);
        }
    }
    void GameObject::clean(){
        for(auto& component : components_){
            component.second->clean();
        }
    }
    void GameObject::handleInput(engine::core::Context& context){
        for(auto& component : components_){
            component.second->handleInput(context);
            
    }
};

}