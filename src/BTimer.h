#ifndef B_TIMER_H
#define B_TIMER_H

/**
 * Helper class for managing timings.
 */
class Timer {
private:
    unsigned long _beginTime;
    unsigned long _duration;
    bool _started = false;

public:
    
    Timer(unsigned long duration = 0);

    bool is_running();

    void set_duration(unsigned long duration);

    void start();

    void start(unsigned long duration);

    /**
     * Stops the timer and returns the remaining time.
     */
    unsigned long stop();

    /**
     * Checks if the timer has finished.
     * Does not restart or stop the timer.
     */
    bool is_finished();
};
#endif
