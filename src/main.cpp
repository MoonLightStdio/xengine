#include "engine/core/game.h"
#include "spdlog/spdlog.h"
int main(){
    spdlog::set_level(spdlog::level::trace); // 设置spdlog的日志级别为trace
    spdlog::warn("this is list branch");
    engine::core::Game game;
    game.run();

}