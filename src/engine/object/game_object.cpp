#include "game_object.h"

namespace engine::object {

    GameObject::GameObject(const std::string& name,const std::string& tag){
        name_ = name;
        tag_ = tag;
    spdlog::trace("GameObject created: {} {}", name_, tag_);

    }
    void GameObject::update(float delta_time){
        for(auto& component : components_){
            component.second->update(delta_time);
        }
    }
    void GameObject::render(){
        for(auto& component : components_){
            component.second->render();
        }
    }
    void GameObject::clean(){
        for(auto& component : components_){
            component.second->clean();
        }
    }
    void GameObject::handleInput(){
        for(auto& component : components_){
            component.second->handleInput();
            
    }
};

}