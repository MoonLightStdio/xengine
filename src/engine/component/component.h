#pragma once

namespace engine::object{
    class GameObject;  //生命game游戏类
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
    //设置为保护函数只有方便子类调用
    virtual void init(){}
    virtual void update(float delta_time){}
    virtual void handleInput(){}
    virtual void render(){}
    virtual void clean() {}
    };
}  //namespace engine::component