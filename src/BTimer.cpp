#include <Arduino.h>
#include "BTimer.h"

Timer::Timer(unsigned long duration) : 
    _duration(duration),
    _started(false)
{ }

bool Timer::is_running() {
    return _started;
}

void Timer::set_duration(unsigned long duration) {
    _duration=duration;
}

void Timer::start() {
    _beginTime=millis();
    _started=true;
}

void Timer::start(unsigned long duration) {
    _duration = duration;
    start();
}

unsigned long Timer::stop() {
    _started=false;
    unsigned long elapsed = (unsigned long)(millis() - _beginTime);
    return (elapsed >= _duration) ? 0 : _duration - elapsed;
}

bool Timer::is_finished() {
    if (!_started){
        return false;
    }
    return (unsigned long)(millis() - _beginTime) >= _duration ;
}
