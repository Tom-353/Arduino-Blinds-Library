#ifndef Blinds_h
#define Blinds_h

#define _TIME_COMMAND_TIMEOUT 500

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

class Timer {
private:
    unsigned long _beginTime;
    unsigned long _duration;

public:
    bool started = false;
    
    Timer(unsigned long duration = 0);

    void set_duration(unsigned long duration);

    void start();

    void stop();

    bool finished();
};

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

    void loop();
};

class ButtonDebounce {
private:
    int _buttonPin;
    Timer _debounceTimer;

public:
    bool pressed = false;

    void begin(int pin);

    void loop();
};

class BlindController {
private:
    MotorController _motor;
    unsigned long _timeUpDown;
    unsigned long _timeTilt;
    ButtonDebounce _buttonUp;
    ButtonDebounce _buttonDown;
    bool _forceStop;

public:
    BlindPosition position = NA;
    BlindPosition positionTarget = NA;
    bool still;

    void begin(int motorPinUp, int motorPinDown, int buttonPinUp,
               int buttonPinDown, unsigned long timeUpDown,
               unsigned long timeTilt, bool forceStop=false, BlindPosition newPosition=NA);

    void go_to(BlindPosition targetPosition);

    void loop();
};

#endif