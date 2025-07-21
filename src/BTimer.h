#ifndef B_TIMER_H
#define B_TIMER_H

/**
 * Helper class for managing timings.
 */
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

    /**
     * Checks if the timer has finished.
     * Does not restart or stop the timer.
     */
    bool finished();
};

#endif
