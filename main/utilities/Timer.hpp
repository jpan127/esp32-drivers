#pragma once
#include <soc/timer_group_struct.h>
#include <driver/timer.h>
#include <driver/periph_ctrl.h>

// Function pointer
typedef (void)(*f_ptr)(void *);

//////////////////////////////////////////////////////////
//  Instructions:                                       //
//      1. Use setter functions to set configuration.   //
//      2. Initialize                                   //
//      3. Start Timer                                  //
//////////////////////////////////////////////////////////

class Timer
{
public:

    // Constructor with default settings
    Timer(timer_group_t timer_group);

    void        Initialize();

    void        SetCounterDirection(timer_count_dir_t direction, timer_idx_t timer_num=TIMER_0);

    void        SetAutoReload(bool reload, timer_idx_t timer_num=TIMER_0);

    void        EnableInterrupt(f_ptr callback, timer_idx_t timer_num=TIMER_0);

    void        SetValue(uint64_t value, timer_idx_t timer_num=TIMER_0);

    void        StartTimer(timer_idx_t timer_num=TIMER_0);

    void        PauseTimer(timer_idx_t timer_num=TIMER_0);

    uint64_t    ReadTimer(timer_idx_t timer_num=TIMER_0);

private:

    timer_group_t   TimerGroup;
    timer_config_t  TimerConfig;
    bool            InterruptMode;
};