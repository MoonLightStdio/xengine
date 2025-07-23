#pragma once
#include <string>
#include <vector>
#include <memory>
namespace engine::core{
    class Context;
}
namespace engine::object{
    class GameObject;
}

namespace engine::resource{
    class ResourceManager;
}

namespace engine::scene{
    
class SceneManager;

class Scene{
protected:
    /// @brief 场景名称
    std::string scene_name_;
    /// @brief 上下文
    engine::core::Context& context_;
    /// @brief 场景管理器
    engine::scene::SceneManager& scene_manager_;
    /// @brief 是否初始化
    bool is_initialized_;
    /// @brief 场景中游戏对象
    std::vector<std::unique_ptr<engine::object::GameObject>> game_objects_;
    /// @brief 待添加游戏对象
    std::vector<std::unique_ptr<engine::object::GameObject>> pending_additions_; 

public:
    /// @brief 构造函数
    /// @param name 场景名称
    /// @param context  上下文
    /// @param scene_manager  场景管理器
    Scene(std::string name,engine::core::Context& context,engine::scene::SceneManager& scene_manager);
    virtual ~Scene();
    
    //禁止拷贝和一栋构造
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

        // 核心循环方法
    virtual void init();                        ///< @brief 初始化场景。
    virtual void update(float delta_time);      ///< @brief 更新场景。
    virtual void render();                      ///< @brief 渲染场景。
    virtual void handleInput();                 ///< @brief 处理输入。
    virtual void clean();                       ///< @brief 清理场景。

    virtual void addGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

    virtual void safeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

    virtual void removeGameObject(engine::object::GameObject* game_object_ptr);

    virtual void safeRemoveGameObject(engine::object::GameObject* game_object_ptr);

    const std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() const {return game_objects_;}

    engine::object::GameObject* findGameObjectByName(const std::string& name) const;
    
    //getters and setters
    const std::string& getName() const {return scene_name_;}
    void setName(const std::string& name) {scene_name_ = name;}
    void setInitialized(bool is_initialized) {is_initialized_ = is_initialized;}
    bool isInitialized() const {return is_initialized_;}

    engine::core::Context& getContext() const {return context_;}
    engine::scene::SceneManager& getSceneManager() const {return scene_manager_;}
    std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() { return game_objects_; } ///< @brief 获取场景中的游戏对象
    

protected:
 void processPendingAdditions();
};
}