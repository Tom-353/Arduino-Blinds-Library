#include <Arduino.h>
#include "BButtonDebounce.h"

void ButtonDebounce::begin(int pin){
    _buttonPin = pin;
    pinMode(_buttonPin,INPUT_PULLUP);
    _debounceTimer.set_duration(10);
}

void ButtonDebounce::loop(){
    if(digitalRead(_buttonPin) == LOW){
        if(_debounceTimer.is_finished()){
            pressed=true;
        }
        else if (!_debounceTimer.is_running()){
            _debounceTimer.start();
        }
    }
    else{
        _debounceTimer.stop();
        pressed=false;
    }
}
