// #include "Timer.hpp"

// Timer::Timer(timer_group_t timer_group)
// {
//     TimerConfig.alarm_en       = false;
//     TimerConfig.counter_en     = true;
//     TimerConfig.intr_type      = TIMER_INTR_LEVEL;
//     TimerConfig.counter_dir    = TIMER_COUNT_DOWN;
//     TimerConfig.auto_reload    = true;
//     TimerConfig.divider        = ;///////////////

//     TimerGroup = timer_group;
// }

// void Timer::Initialize()
// {
//     ESP_ERROR_CHECK(timer_init(TimerGroup, timer_num, &TimerConfig));
// }

// void Timer::EnableInterrupt(f_ptr callback, timer_idx_t timer_num)
// {
//     // Register ISR
//     ESP_ERROR_CHECK(timer_isr_register(TimerGroup, timer_num, callback, NULL, 0, NULL));

//     // Enable interrupt
//     ESP_ERROR_CHECK(timer_enable_intr(TimerGroup, timer_num));
// }

// void Timer::SetCounterDirection(timer_count_dir_t direction, timer_idx_t timer_num)
// {
//     ESP_ERROR_CHECK(timer_set_counter_mode(TimerGroup, timer_num, direction));
// }

// void Timer::SetAutoReload(bool reload, timer_idx_t timer_num)
// {
//     // TimerConfig.auto_reload = reload;
//     (reload) ? (ESP_ERROR_CHECK(timer_set_auto_reload(TimerGroup, timer_num, TIMER_AUTORELOAD_EN)))
//              : (ESP_ERROR_CHECK(timer_set_auto_reload(TimerGroup, timer_num, TIMER_AUTORELOAD_DIS)));
// }

// void Timer::SetValue(uint64_t value, timer_num)
// {
//     ESP_ERROR_CHECK(timer_set_alarm_value(TimerGroup, timer_num, value));
// }

// void Timer::StartTimer(timer_idx_t timer_num)
// {
//     ESP_ERROR_CHECK(timer_start(TimerGroup, timer_num));
// }

// void Timre::PauseTimer(timer_idx_t timer_num)
// {
//     ESP_ERROR_CHECK(timer_pause(TimerGroup, timer_num));
// }

// uint64_t Timer::ReadTimer(timer_idx_t timer_num)
// {
//     uint64_t time = -1;
//     ESP_ERROR_CHECK(timer_get_counter_value(TimerGroup, timer_num, &time));
//     return time;
// }