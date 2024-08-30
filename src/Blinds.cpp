#include <Arduino.h>
#include "Blinds.h"

Timer::Timer(unsigned long duration=0) {
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

void ButtonDebounce::begin(int pin){
    _buttonPin = pin;
    pinMode(_buttonPin,INPUT_PULLUP);
    _debounceTimer.set_duration(10);
}

void ButtonDebounce::loop(){
    if(digitalRead(_buttonPin) == LOW){
        if(_debounceTimer.finished()){
            pressed=true;
        }
        else if (!_debounceTimer.started){
            _debounceTimer.start();
        }
    }
    else{
        _debounceTimer.stop();
        pressed=false;
    }
}

void BlindController::begin(int motorPinUp, int motorPinDown, int buttonPinUp,
                           int buttonPinDown, unsigned long timeUpDown,
                           unsigned long timeTilt, bool forceStop=false, BlindPosition newPosition=NA) {
    _motor.begin(motorPinUp, motorPinDown);
    _buttonUp.begin(buttonPinUp);
    _buttonDown.begin(buttonPinDown);
    _timeUpDown = timeUpDown;
    _timeTilt = timeTilt;
    position = newPosition;
    _forceStop = forceStop;
}

void BlindController::go_to(BlindPosition targetPosition){
    switch (targetPosition)
    {
    case NA:return;break;
    case UP:_motor.move_up(_timeUpDown);  position=UP;break;
    case DOWN:switch (position){
        case DEG90:_motor.move_down(_timeTilt/2);  position=DEG45;break;
        case DEG45:_motor.move_down(_timeTilt/2);  position=DOWN;break;
        default:_motor.move_down(_timeUpDown);position=DOWN;break;
    }break;
    case DEG45:switch (position){
        case DEG90:_motor.move_down(_timeTilt/2);  position=DEG45;break;
        case DOWN:_motor.move_up(_timeTilt/2);     position=DEG45;break;
        default:_motor.move_down(_timeUpDown);position=DOWN;break;
    }break;
    case DEG90:switch (position){
        case DEG45:_motor.move_up(_timeTilt/2);    position=DEG90;break;
        case DOWN:_motor.move_up(_timeTilt/2);     position=DEG45;break;
        default:_motor.move_down(_timeUpDown);position=DOWN;break;
    }break;
    }
}

void BlindController::loop(){
    _motor.loop();
    still=_motor.available;
    _buttonUp.loop();
    _buttonDown.loop();
    if(positionTarget==position){
        positionTarget = NA;
    }
    if(_motor.available){
        if(positionTarget!=NA){
            go_to(positionTarget);
        }
        else if(_buttonUp.pressed){ // provádí příkaz z tlačítka
            if     (position==UP)   {} // nic
            else if(position==DEG45){_motor.move_up(_timeTilt/2); position=DEG90;}//otoč na deg90
            else if(position==DOWN) {_motor.move_up(_timeTilt/2); position=DEG45;}//otoč na deg45
            else                    {_motor.move_up(_timeUpDown); position=UP;}//vyjede uplne nahoru
        }
        else if(_buttonDown.pressed){ // provádí příkaz z tlačítka
            if     (position==DOWN) {} // nic
            else if(position==DEG90){_motor.move_down(_timeTilt/2); position=DEG45;}//otoč na deg45
            else if(position==DEG45){_motor.move_down(_timeTilt/2); position=DOWN;} //sjet úplně dolů
            else                    {_motor.move_down(_timeUpDown); position=DOWN;} //sjet úplně dolů
        }
    }
    else if(_forceStop && ((position==DOWN && _buttonUp.pressed) || (position==UP && _buttonDown.pressed))){
        if(_motor.stop()){
            position = NA;
            positionTarget = NA;
        }
    }
}
