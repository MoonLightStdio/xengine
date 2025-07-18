#pragma once

#include "../component/component.h"
#include <memory>
#include "unordered_map"
#include <typeindex>   //用于类型索引
#include <utility>     //用于完美转发

#include "spdlog/spdlog.h"

namespace engine::object{
class GameObject final{
private:
    std::string name_;
    std::string tag_;
    std::unordered_map<std::type_index,std::unique_ptr<engine::component::Component>> components_;
    bool need_remove_=false;

public:
    GameObject(const std::string& name = "",const std::string& tag = "");

    //禁止拷贝
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    //禁止移动
    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;

    //setters and getters
    void setName(const std::string& name) { name_ = name; }
    void setTag(const std::string& tag) { tag_ = tag; }
    const std::string& getName() const { return name_; }
    const std::string& getTag() const { return tag_; }
    void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }
    bool isNeedRemove() const { return need_remove_; }

    //添加组件
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        //这个组件T必须继承自Component
        static_assert(std::is_base_of<engine::component::Component, T>::value, "T must be derived from Component");
        auto type_index = std::type_index(typeid(T));
        

        if(hasCompontent<T>()){
            return getComponent<T>();
        }
        auto new_component = std::make_unique<T>(std::forward<Args>(args)...);
        T *ptr = new_component.get();
        components_[type_index] = std::move(new_component);
        spdlog::debug("Add component {} to GameObject {}", typeid(T).name(), name_);
        return ptr;
    }
    
    template <typename T>
    bool hasCompontent() const {
        static_assert(std::is_base_of<engine::component::Component,T>::value, "T must be derived from Component");
        return components_.contains(std::type_index(typeid(T)));
    }
    template <typename T>
    T* getComponent() const {
        static_assert(std::is_base_of<engine::component::Component,T>::value, "T must be derived from Component");
        auto type_index = std::type_index(typeid(T));
        auto it = components_.find(type_index);
        if(it != components_.end()){
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template <typename T>
    void removeComponent(){
        static_assert(std::is_base_of<engine::component::Component,T>::value, "T must be derived from Component");
        auto type_index = std::type_index(typeid(T));
        auto it = components_.find(type_index);
        if(it != components_.end()){
            it->second->clean();
            components_.erase(it);
        }
    }
    void update(float delta_time);
    void render();
    void clean();
    void handleInput();
};
}