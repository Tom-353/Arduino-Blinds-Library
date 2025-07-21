#include <Arduino.h>
#include "Blinds.h"

BlindController::BlindController(int motorPinUp, int motorPinDown, int buttonPinUp,
                           int buttonPinDown, unsigned long timeUpDown,
                           unsigned long timeTilt, bool forceStop, BlindPosition newPosition) :
    _buttonUp(buttonPinUp),
    _buttonDown(buttonPinDown),
    _timeUpDown(timeUpDown),
    _timeTilt(timeTilt),
    _position(newPosition),
    _forceStop(forceStop),
    _motor(motorPinUp, motorPinDown)
{ }

BlindPosition BlindController::get_position() {
    return _position;
}

bool BlindController::is_still() {
    return _still;
}

void BlindController::go_to(BlindPosition targetPosition){
    switch (targetPosition)
    {
        case NA:return;break;
        case UP:_motor.move_up(_timeUpDown);  _position=UP;break;
        case DOWN:switch (_position){
            case DEG90: _motor.move_down(_timeTilt/2); _position=DEG45;break;
            case DEG45: _motor.move_down(_timeTilt/2); _position=DOWN;break;
            default:    _motor.move_down(_timeUpDown); _position=DOWN;break;
        }break;
        case DEG45:switch (_position){
            case DEG90: _motor.move_down(_timeTilt/2); _position=DEG45;break;
            case DOWN:  _motor.move_up(_timeTilt/2);   _position=DEG45;break;
            default:    _motor.move_down(_timeUpDown); _position=DOWN;break;
        }break;
        case DEG90:switch (_position){
            case DEG45: _motor.move_up(_timeTilt/2);   _position=DEG90;break;
            case DOWN:  _motor.move_up(_timeTilt/2);   _position=DEG45;break;
            default:    _motor.move_down(_timeUpDown); _position=DOWN;break;
        }break;
    }
}

void BlindController::go_up() {
    if     (_position==UP)   {} // nic
    else if(_position==DEG45){_motor.move_up(_timeTilt/2); _position=DEG90;}//otoč na deg90
    else if(_position==DOWN) {_motor.move_up(_timeTilt/2); _position=DEG45;}//otoč na deg45
    else                     {_motor.move_up(_timeUpDown); _position=UP;}//vyjede uplne nahoru
}

void BlindController::go_down() {
    if     (_position==DOWN) {} // nic
    else if(_position==DEG90){_motor.move_down(_timeTilt/2); _position=DEG45;}//otoč na deg45
    else if(_position==DEG45){_motor.move_down(_timeTilt/2); _position=DOWN;} //sjet úplně dolů
    else                     {_motor.move_down(_timeUpDown); _position=DOWN;} //sjet úplně dolů
}

void BlindController::loop(){
    _motor.loop();
    _still = _motor.is_available();
    _buttonUp.loop();
    _buttonDown.loop();
    if(positionTarget == _position){
        positionTarget = NA;
    }
    if(_motor.is_available()){
        if(positionTarget != NA){
            go_to(positionTarget);
        }
        else if(_buttonUp.is_pressed()) {
            go_up();
        }
        else if(_buttonDown.is_pressed()) {
            go_down();
        }
    } 
    else if((_position == DOWN and _buttonUp.is_pressed()) or
            (_position == UP and _buttonDown.is_pressed())) {
        stop();
    }
}

void BlindController::stop() {
    if (_forceStop) {
        if(_motor.stop()) {
            _position = NA;
            positionTarget = NA;
        }
    }
}
