#pragma once

namespace engine::object{
    class GameObject;  //生命game游戏类
}

namespace engine::core{
    class Context;  //生命上下文类
}

namespace engine::component{
    class Component{
        friend class engine::object::GameObject;

protected:
    engine::object::GameObject* owner_=nullptr; //拥有者

public:
    Component() = default;  //构造函数为空，Component类为基类
    virtual ~Component()=default; //自动调用子类的析构函数，确保正确释放
    
    //删除拷贝构造函数和赋值运算符，防止拷贝
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    //删除移动构造函数和赋值运算符，防止移动
    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;

    void setOwner(engine::object::GameObject* owner){
        owner_ = owner;
    }
    engine::object::GameObject* getOwner(){
        return owner_;
    }

protected:
   // 关键循环函数，全部设为保护，只有 GameObject 需要（可以）调用
    virtual void init() {}                      ///< @brief 保留两段初始化的机制，GameObject 添加组件时自动调用，不需要外部调用
    virtual void handleInput(engine::core::Context&) {}                 ///< @brief 处理输入
    virtual void update(float, engine::core::Context&) = 0;             ///< @brief 更新，必须实现
    virtual void render(engine::core::Context&) {}                      ///< @brief 渲染
    virtual void clean() {}                                             ///< @brief 清理
    };
}  //namespace engine::component