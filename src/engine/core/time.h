#pragma once
#include "SDL3/SDL_stdinc.h"

namespace engine::core{
class Time{
private:
Uint64 start_time_;
Uint64 end_time_;
float dt_;
float fps_;


public:
Time();
~Time();

Time(const Time&)=delete;
Time& operator=(const Time&)=delete;
Time(Time&&)=delete;
Time& operator=(Time&&)=delete;
void setFps(const int fps);
float tick();
private:
};
}