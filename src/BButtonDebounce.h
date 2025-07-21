#ifndef B_BUTTON_DEBUNCE
#define B_BUTTON_DEBUNCE

#include "BTimer.h"

class ButtonDebounce {
private:
    int _buttonPin;
    Timer _debounceTimer;

public:
    bool pressed = false;

    void begin(int pin);

    /**
     * Process timers.
     */
    void loop();
};
#endif
