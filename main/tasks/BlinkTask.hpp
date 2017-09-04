#pragma once
#include "utilities.h"
#include "Task.hpp"
#include "gpio.hpp"

class BlinkTask : public Task
{
public:

    BlinkTask(std::string task_name="BlinkTask", 
                                            uint16_t stack_size=2048,
                                            uint8_t priority=5) : 
                                            Task(task_name, stack_size, priority)
    {
        /* EMPTY */
    }

    void Run(void *p)
    {
        // Initialize
        GpioOutput blinker(GPIO_NUM_23);
        
        while (1)
        {
            blinker.Set();      Delay(1000);
            blinker.Clear();    Delay(1000);

            ESP_LOGI("BlinkTask", "Blink!");
        }        
    }
};