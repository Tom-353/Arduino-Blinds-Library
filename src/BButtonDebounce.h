#ifndef B_BUTTON_DEBUNCE
#define B_BUTTON_DEBUNCE

#include "BTimer.h"

class ButtonDebounce {
private:
    int _buttonPin;
    Timer _debounceTimer;
    bool _pressed = false;

public:
    ButtonDebounce(int pin);

    bool is_pressed();

    /**
     * Process timers.
     */
    void loop();
};
#endif
