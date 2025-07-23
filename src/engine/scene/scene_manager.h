#pragma once

#include <memory>
#include <string>
#include <vector>


namespace engine::core{
    class Context;
}

namespace engine::scene{
    class Scene;

class SceneManager final{
private:  
    engine::core::Context& context_;   ///< @brief 游戏上下文
    std::vector<std::unique_ptr<Scene>> scene_stack_; ///< @brief 场景栈


    enum class PendingAction {None, Push, Pop, Replace};   ///< @brief 待处理动作
    PendingAction pending_action_=PendingAction::None;   ///< @brief 待处理动作
    std::unique_ptr<Scene> pending_scene_; ///< @brief 待处理场景

public:
    /// @brief 创建场景管理器,防止隐士转换
    /// @param context  游戏上下文
    explicit SceneManager(engine::core::Context& context);
    ~SceneManager();

    //禁止拷贝和移动
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    // 延时切换场景
    /// @brief 请求压入场景 
    void requestPushScene(std::unique_ptr<Scene>&& scene);
    /// @brief 请求切换场景
    void requestPopScene();
    /// @brief 请求替换场景
    /// @param scene 替换的场景
    void requestReplaceScene(std::unique_ptr<Scene>&& scene);

    //getters
    /// @brief 获取当前场景
    Scene* getCurrentScene() const ;
    /// @brief 获取游戏上下文
    /// @return Context& 当前游戏上下文
    engine::core::Context& getContext() const {return context_;}


    void update(float delta_time);
    void render();
    void handleInput();
    void close();

private:
    /// @brief 处理待处理动作
    void processPendingActions();
    void pushScene(std::unique_ptr<Scene>&& scene);
    void popScene();
    void replaceScene(std::unique_ptr<Scene>&& scene);
};
}

