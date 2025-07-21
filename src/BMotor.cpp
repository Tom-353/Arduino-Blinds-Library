#include <Arduino.h>
#include "BMotor.h"

void MotorController::begin(int pinUp,int pinDown) {
    _pinUp   = pinUp;
    _pinDown = pinDown;
    pinMode(_pinUp,   OUTPUT);
    pinMode(_pinDown, OUTPUT);
    digitalWrite(_pinUp,   HIGH);
    digitalWrite(_pinDown, HIGH);
    _RelayEndTimeout.set_duration(_TIME_COMMAND_TIMEOUT);
    _RelayBeginTimeout.set_duration(100);
}

bool MotorController::stop(){
    if(!_RelayBeginTimeout.finished()){
        return false;
    }
    digitalWrite(_pinDown, HIGH);
    digitalWrite(_pinUp,   HIGH);
    _MovementTimer.stop();
    _RelayBeginTimeout.stop();
    _RelayEndTimeout.start();
    return true;
}

void MotorController::move(unsigned long time_ms, bool up){
    if(!available){return;}
    if(up){
        digitalWrite(_pinDown, HIGH);
        digitalWrite(_pinUp,   LOW);
    }
    else {
        digitalWrite(_pinDown, LOW);
        digitalWrite(_pinUp,   HIGH);
    }
    _MovementTimer.set_duration(time_ms);
    _MovementTimer.start();
    available=false;
    _RelayBeginTimeout.start();
}

void MotorController::move_up(unsigned long time_ms){
    move(time_ms,true);
}

void MotorController::move_down(unsigned long time_ms){
    move(time_ms,false);
}

void MotorController::loop(){
    if (_MovementTimer.finished()){
        stop();
    }
    if (_RelayEndTimeout.finished()){
        _RelayEndTimeout.stop();
        available=true;
    }
}
