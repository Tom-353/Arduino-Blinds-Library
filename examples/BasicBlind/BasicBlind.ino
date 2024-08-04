
#include <Blinds.h>

BlindController myBlind;

void setup(){
    myBlind.begin(21,2,37,39,40000,1300);
}

void loop(){
    myBlind.loop();
}