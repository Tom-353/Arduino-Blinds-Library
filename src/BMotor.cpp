#include <Arduino.h>
#include "BMotor.h"

MotorController::MotorController(int pinUp,int pinDown) :
    _pinUp(pinUp),
    _pinDown(pinDown),
    _RelayEndTimeout(_TIME_COMMAND_TIMEOUT),
    _RelayBeginTimeout(100)
{
    pinMode(_pinUp,   OUTPUT);
    pinMode(_pinDown, OUTPUT);
    digitalWrite(_pinUp,   HIGH);
    digitalWrite(_pinDown, HIGH);
}

bool MotorController::is_available() {
    return _available;
}

bool MotorController::stop(){
    if(!_RelayBeginTimeout.is_finished()) {
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
    if(!_available) {
        return;
    }
    if(up) {
        digitalWrite(_pinDown, HIGH);
        digitalWrite(_pinUp,   LOW);
    }
    else {
        digitalWrite(_pinDown, LOW);
        digitalWrite(_pinUp,   HIGH);
    }
    _MovementTimer.set_duration(time_ms);
    _MovementTimer.start();
    _available=false;
    _RelayBeginTimeout.start();
}

void MotorController::move_up(unsigned long time_ms) {
    move(time_ms,true);
}

void MotorController::move_down(unsigned long time_ms) {
    move(time_ms,false);
}

void MotorController::loop() {
    if (_MovementTimer.is_finished()) {
        stop();
    }
    if (_RelayEndTimeout.is_finished()) {
        _RelayEndTimeout.stop();
        _available=true;
    }
}
