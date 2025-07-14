#include "time.h"
#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"
namespace engine::core{
Time::Time(){
    start_time_=SDL_GetTicksNS();
    end_time_=start_time_;
}
Time::~Time()
{
}
void Time::setFps(int fps)
{
    fps_=fps;
    dt_=1000000000/fps;
    spdlog::info("dt_:{}",dt_);
}
float Time::tick()
{
    end_time_=SDL_GetTicksNS();
    if(end_time_-start_time_<dt_){
        SDL_DelayNS(dt_-(end_time_-start_time_));
        
        //spdlog::trace("帧率：{}",fps_);
        start_time_=end_time_;
        return 1/fps_;
        
    }else{
        float tmp=1000000000/(end_time_-start_time_);
        //spdlog::trace("帧率：{}",tmp);
        start_time_=end_time_;
        return 1/tmp;

    }

}
}