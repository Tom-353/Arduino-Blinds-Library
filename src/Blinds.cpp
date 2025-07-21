#include <Arduino.h>
#include "Blinds.h"

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
    else if((position==DOWN && _buttonUp.pressed) || (position==UP && _buttonDown.pressed)) {
        this->stop();
    }
}

void BlindController::stop() {
    if (_forceStop) {
        if(_motor.stop()){
            position = NA;
            positionTarget = NA;
        }
    }
}
