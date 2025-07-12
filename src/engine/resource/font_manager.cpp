#include "font_manager.h"
#include "spdlog/spdlog.h"
#include "stdexcept"
namespace engine::resource
{
FontManager::FontManager(){
    if (!TTF_WasInit() && !TTF_Init()) {
    }
    spdlog::trace("FontManager 构造成功。");
}
FontManager::~FontManager(){
    TTF_Quit();
    spdlog::trace("FontManager 析构成功。");
}
TTF_Font* FontManager::getFont(const std::string& fontPath, int fontSize){
    FontKey key = {fontPath, fontSize};
    auto it = fonts_.find(key);
    if (it != fonts_.end()) {
        return it->second.get();
    }
    spdlog::warn("字体 '{}' ({}pt) 未找到,尝试加载字体", fontPath, fontSize);
    return loadFont(fontPath, fontSize);
}
TTF_Font* FontManager::loadFont(const std::string& fontPath, int fontSize){
        // 检查点大小是否有效
    if (fontSize <= 0) {
        spdlog::error("无法加载字体 '{}'：无效的点大小 {}。", fontPath, fontSize);
        return nullptr;
    }

    // 创建映射表的键
    FontKey key = {fontPath, fontSize};

    // 首先检查缓存
    auto it = fonts_.find(key);
    if (it != fonts_.end()) {
        return it->second.get();
    }

    // 缓存中不存在，则加载字体
    spdlog::debug("正在加载字体：{} ({}pt)", fontPath, fontSize);
    TTF_Font* raw_font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!raw_font) {
        spdlog::error("加载字体 '{}' ({}pt) 失败：{}", fontPath, fontSize, SDL_GetError());
        return nullptr;
    }

    // 使用 unique_ptr 存储到缓存中
    fonts_.emplace(key, std::unique_ptr<TTF_Font, SDLFontDeleter>(raw_font));
    spdlog::debug("成功加载并缓存字体：{} ({}pt)", fontPath, fontSize);
    return raw_font;
}

void FontManager::unloadFont(const std::string& fontPath, int fontSize){
    FontKey key = {fontPath, fontSize};
    auto it = fonts_.find(key);
    if (it != fonts_.end()) {
        fonts_.erase(it);
        spdlog::debug("字体 '{}' ({}pt) 已卸载", fontPath, fontSize);
    }else{
        spdlog::warn("尝试卸载不存在的字体");
    }
}

void FontManager::clearFonts(){
    if(!fonts_.empty()){
        spdlog::debug("正在卸载所有字体");
        fonts_.clear();
    }
}





}