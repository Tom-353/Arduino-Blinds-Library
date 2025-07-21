#include <Arduino.h>
#include "BButtonDebounce.h"

ButtonDebounce::ButtonDebounce(int pin) :
    _buttonPin(pin),
    _debounceTimer(10)
{
    pinMode(_buttonPin, INPUT_PULLUP);
}

bool ButtonDebounce::is_pressed() {
    return _pressed;
}

void ButtonDebounce::loop(){
    if(digitalRead(_buttonPin) == LOW){
        if(_debounceTimer.is_finished()){
            _pressed=true;
        }
        else if (!_debounceTimer.is_running()){
            _debounceTimer.start();
        }
    }
    else {
        _debounceTimer.stop();
        _pressed=false;
    }
}
