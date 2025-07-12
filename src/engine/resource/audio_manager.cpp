#include "audio_manager.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace engine::resource
{
    AudioManager::AudioManager()
    {
       // 使用所需的格式初始化SDL_mixer（推荐OGG、MP3）
    MIX_InitFlags flags = MIX_INIT_OGG | MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags) {
    }

    // SDL3打开音频设备的方法。默认值：44100 Hz，默认格式，2声道（立体声），2048采样块大小
    if (!Mix_OpenAudio(0, nullptr)) {
        Mix_Quit(); // 如果OpenAudio失败，先清理Mix_Init，再抛出异常
    }
    spdlog::trace("AudioManager 构造成功。");
    }

    AudioManager::~AudioManager()
    {
        // 立即停止所有音频播放
    Mix_HaltChannel(-1); // 停止所有音效
    Mix_HaltMusic();     // 停止音乐

    // 清理资源映射（unique_ptrs会调用删除器）
    clearChunks();
    clearMusics();

    // 关闭音频设备
    Mix_CloseAudio();

    // 退出SDL_mixer子系统
    Mix_Quit();
    spdlog::trace("AudioManager 析构成功。");
    }

    Mix_Chunk *AudioManager::loadChunk(const std::string &path)
    {
        auto it = chunks_.find(path);
    if (it != chunks_.end()) {
        return it->second.get();
    }
    Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
    if (chunk == nullptr) {
        spdlog::error("AudioManager 错误: 无法加载音效文件: {}", path);
    }
    chunks_.emplace(path, std::unique_ptr<Mix_Chunk,SDLMixerChunkDeleter>(chunk));
    return chunk;
    }
    Mix_Chunk *AudioManager::getChunk(const std::string &path)
    {
        auto it = chunks_.find(path);
    if (it != chunks_.end()) {
        return it->second.get();
    }
    spdlog::debug("尝试加载音效文件：{}", path);
    return loadChunk(path);
    }
    void AudioManager::unloadChunk(const std::string &path)
    {
        auto it = chunks_.find(path);
    if (it != chunks_.end()) {
        chunks_.erase(it);
        spdlog::info("成功卸载音效文件：{}", path);
    }
    else{
        spdlog::debug("尝试卸载不存在的音效文件：{}", path);
    }
    }
    void AudioManager::clearChunks()
    {
        if(!chunks_.empty()){
            spdlog::debug("正在清理音效文件...");
            chunks_.clear();
        }
    }



    void AudioManager::unloadMusic(const std::string &path)
    {
        auto it = musics_.find(path);
    if (it != musics_.end()) {
        musics_.erase(it);
    }
    else{
        spdlog::debug("尝试卸载不存在的音乐文件：{}", path);
    }
        
    }

    Mix_Music *AudioManager::getMusic(const std::string &path)
    {
        auto it = musics_.find(path);
    if (it != musics_.end()) {
        return it->second.get();
    }
    spdlog::debug("尝试加载音乐文件：{}", path);
    return loadMusic(path);
    }
    Mix_Music *AudioManager::loadMusic(const std::string &path)
    {
        auto it = musics_.find(path);
    if (it != musics_.end()) {
        return it->second.get();
    }
    Mix_Music *music = Mix_LoadMUS(path.c_str());
    if (music == nullptr) {
        spdlog::error("AudioManager 错误: 无法加载音乐文件: {}", path);
    }
    musics_.emplace(path, std::unique_ptr<Mix_Music,SDLMixerMusicDeleter>(music));
    return music;
    }

    void AudioManager::clearMusics(){
        if(!musics_.empty()){
            spdlog::debug("正在清理音乐文件...");
            musics_.clear();
    }
}

    
void AudioManager::clearMixs()
{
    clearChunks();
    clearMusics();
}

}

