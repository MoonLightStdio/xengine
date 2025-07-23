cmake_minimum_required(VERSION 3.10.0)
project(xengine VERSION 0.1.0 LANGUAGES C CXX)


# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(xengine
src/main.cpp 
src/engine/core/game.cpp
src/engine/core/time.cpp
src/engine/resource/texture_manager.cpp
src/engine/resource/font_manager.cpp
src/engine/resource/audio_manager.cpp
src/engine/resource/resource_manager.cpp
src/engine/render/camera.cpp
src/engine/render/renderer.cpp
src/engine/core/config.cpp
src/engine/input/input_manager.cpp
src/engine/object/game_object.cpp
src/engine/core/context.cpp
src/engine/component/sprite_component.cpp
src/engine/component/transform_component.cpp
src/game/scene/game_scene.cpp
src/engine/scene/scene_manager.cpp
src/engine/scene/scene.cpp
)
# 指定目标的头文件目录
target_include_directories(xengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)





target_link_libraries(xengine PRIVATE  
${CMAKE_CURRENT_SOURCE_DIR}/lib/hv.lib 
${CMAKE_CURRENT_SOURCE_DIR}/lib/hv_static.lib
${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3_mixer.lib
${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3_image.lib
${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3_ttf.lib
${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3.lib
)