#include "config.h"
#include <fstream>
#include "spdlog/spdlog.h"
namespace engine::core{
Config::Config(std::string path)
{
    loadFromFile(path);
}
Config::~Config()
{
}
bool Config::loadFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open()){
        spdlog::warn("Failed to open config file: {},try to use default config", path);
        if(!saveToFile(path)){
            spdlog::error("Failed to save default config file: {}", path);
            return false;
        }
        return false;
    }
    try{
nlohmann::json j;
    file >> j;
    fromJson(j);
    spdlog::info("Load config file: {}", path);
    return true;
    }catch (const std::exception &e){
        spdlog::error("Failed to load config file: {}, error: {}", path, e.what());
        return false;
    }
}
bool Config::saveToFile(const std::string &path) const
{
    std::ofstream file(path);
    if(!file.is_open()){
        spdlog::error("Failed to save config file: {}", path);
        return false;
    }
    try{
       nlohmann::ordered_json j=toJson();
        file <<j.dump(4);
        spdlog::info("Save config file: {}", path);
        return true;
    }catch (const std::exception &e){
        spdlog::error("Failed to save config file: {}, error: {}", path, e.what());
    }
    return false;
}
void Config::fromJson(const nlohmann::json &j)
{
    if(j.contains("window")){
        const auto& window_config = j["window"];
        window_title_= window_config.value("title",window_title_);
        window_width_ = window_config.value("width",window_width_);
        window_height_ = window_config.value("height",window_height_);
        window_resizable_ = window_config.value("resizable",window_resizable_);
    }
    if(j.contains("graphics")){
        const auto& graphics_config = j["graphics"];
        vsync_enabled_ = graphics_config.value("vsync",vsync_enabled_);
    }
    if(j.contains("performance")){
        const auto& performance_config = j["performance"];
        target_fps_ = performance_config.value("target_fps",target_fps_);
        if(target_fps_ < 0){
            spdlog::warn("Target FPS is less than 0, set to 0");
            target_fps_ = 0;
        }
    }
    if(j.contains("audio")){
        const auto& audio_config = j["audio"];
        music_volume_ = audio_config.value("music_volume",music_volume_);
        sound_volume_ = audio_config.value("sound_volume",sound_volume_);
    }
    if(j.contains("input_mappings")&&j["input_mappings"].is_object()){
        const auto& mappings_json = j["input_mappings"];
        try{
            auto input_mappings = mappings_json.get<std::unordered_map<std::string,std::vector<std::string>>>();
            input_mappings_ = std::move(input_mappings);
            spdlog::trace("Load input mappings from config file");
        }catch (const std::exception &e){
            spdlog::error("Failed to load input mappings from config file, error: {}", e.what());
        }
    }else{
        spdlog::warn("No input mappings found in config file");
    }

}
nlohmann::ordered_json Config::toJson() const
{
    return nlohmann::ordered_json{
        {"window", {
            {"title", window_title_},
            {"width", window_width_},
            {"height", window_height_},
            {"resizable", window_resizable_}
        }},
        {"graphics", {
            {"vsync", vsync_enabled_}
        }},
        {"performance", {
            {"target_fps", target_fps_}
        }},
        {"audio", {
            {"music_volume", music_volume_},
            {"sound_volume", sound_volume_}
        }},
        {"input_mappings", input_mappings_}
    };
}
}
