#include <Arduino.h>
#include "BTimer.h"

Timer::Timer(unsigned long duration) {
    _duration=duration;
    started = false;
}

void Timer::set_duration(unsigned long duration) {
    _duration=duration;
}

void Timer::start() {
    _beginTime=millis();
    started=true;
}

void Timer::stop() {
    started=false;
}

bool Timer::finished() {
    if (!started){
        return false;
    }
    return (unsigned long)(millis() - _beginTime) >= _duration ;
}
