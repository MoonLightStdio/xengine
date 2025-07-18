#pragma once 
#include <string>
#include <unordered_map>
#include "json.hpp"
#include "glm/glm.hpp"
namespace engine::core {
class Config{
private:


public:
std::string window_title_="SunnyLand";
int window_width_=1280;
int window_height_=720;

bool window_resizable_ = true;

    // 图形设置
    bool vsync_enabled_ = true;             ///< @brief 是否启用垂直同步

    // 性能设置
    int target_fps_ = 144;                  ///< @brief 目标 FPS 设置，0 表示不限制

    // 音频设置
    float music_volume_ = 0.5f;
    float sound_volume_ = 0.5f;

       // 存储动作名称到 SDL Scancode 名称列表的映射
    std::unordered_map<std::string, std::vector<std::string>> input_mappings_ = {
        // 提供一些合理的默认值，以防配置文件加载失败或缺少此部分
        {"move_left", {"A", "Left"}},
        {"move_right", {"D", "Right"}},
        {"move_up", {"W", "Up"}},
        {"move_down", {"S", "Down"}},
        {"jump", {"J", "Space"}},
        {"attack", {"K", "MouseLeft"}},
        {"pause", {"P", "Escape"}},
        // 可以继续添加更多默认动作
    };

    explicit Config(std::string path); //explicit 防止隐式转换

    ~Config();

    bool loadFromFile(const std::string& path);
    [[nodiscard]] bool saveToFile(const std::string& path) const;

private:
   void fromJson(const nlohmann::json& j);                           ///< @brief 从 JSON 对象反序列化配置。
    nlohmann::ordered_json toJson() const;    
};
}