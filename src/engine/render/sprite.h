#pragma once
#include <optional>
#include "SDL3/SDL_rect.h"
#include <string>
namespace engine::render {
/*
@brief 存储是否翻转、texture_id,原矩形
获取精灵图
@param texture_id 纹理路径
@param rect 想要截取的纹理的矩形
@param is_filp 是否翻转
*/ 
class Sprite {
private:
    std::optional<SDL_FRect> rect_;
    std::string texture_id_;
    bool is_filp_=false;

public:
    /// @brief 创建精灵图
    /// @param texture_id 纹理路径 
    /// @param rect 原矩形
    /// @param is_filp 是否翻转 
    Sprite(std::string texture_id, const std::optional<SDL_FRect>& rect=std::nullopt,bool is_filp=false){
        this->rect_=rect;
        this->texture_id_=texture_id;
        this->is_filp_=is_filp;
    }
    std::string getTextureId() const {
        return texture_id_;
    }
    const std::optional<SDL_FRect>& getSrcRect() const {
        return rect_;
    }
    bool isFilp() const {
        return is_filp_;
    }
    void setFilp(bool is_filp) {
        this->is_filp_ = is_filp;
    }
    void setResRect(const std::optional<SDL_FRect>& rect) {
        this->rect_ = rect;
    }
    void setTextureId(const std::string& texture_id) {
        this->texture_id_ = texture_id;
    }


    
};
}