#ifndef BLINDS_H
#define BLINDS_H

#include "BMotor.h"
#include "BButtonDebounce.h"

/*
UP->DOWN UD
90->45 4590
45->DOWN D45

DOWN->45 D45
45->90 4590
90->UP UD
!DOWN->UP UD
*/
enum BlindPosition {
    NA = 0,
    DOWN = 1,
    DEG45 = 2,
    DEG90 = 3,
    UP = 4
};

class BlindController {
private:
    MotorController _motor;
    unsigned long _timeUpDown;
    unsigned long _timeTilt;
    ButtonDebounce _buttonUp;
    ButtonDebounce _buttonDown;
    bool _forceStop;
    bool _still;
    BlindPosition _position = NA;

public:
    BlindPosition positionTarget = NA;

    /**
     * Initialize the blind.
     * @param timeUpDown The time in milliseconds it takes for the blind to move from the top to the bottom.
     * @param timeTilt The time in milliseconds it takes for the blind to tilt from horizontal to vertical.
     * @param forceStop Whether to enable stoping the blind while it is moving.
     * @param newPosition The position the blind was in before initialization.
     */
    BlindController(int motorPinUp, int motorPinDown, int buttonPinUp,
                    int buttonPinDown, unsigned long timeUpDown,
                    unsigned long timeTilt, bool forceStop=false, BlindPosition newPosition=NA);

    BlindPosition get_position();

    bool is_still();

    /**
     * Tell the blind to go to a predefined position.
     * @param targetPosition The position to go to.
     */
    void go_to(BlindPosition targetPosition);

    void go_up();

    void go_down();

    /**
     * Immediately stops the blind.
     */
    void stop();

    /**
     * Process all timers and buttons.
     * Make sure to call this method as often as possible.
     */ 
    void loop();
};
#endif
