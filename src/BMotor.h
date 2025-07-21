#ifndef B_MOTOR_H
#define B_MOTOR_H

#include "BTimer.h"

class MotorController {
private:
    int _pinUp;
    int _pinDown;
    Timer _MovementTimer;
    Timer _RelayBeginTimeout;
    Timer _RelayEndTimeout;

public:
    bool available = true;

    void begin(int pinUp, int pinDown);

    bool stop();

    void move(unsigned long time_ms, bool up);

    void move_up(unsigned long time_ms);

    void move_down(unsigned long time_ms);

    /**
     * Process timers.
     */
    void loop();
};
#endif
