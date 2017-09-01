#pragma once
#include "utilities.h"
#include "gpio.hpp"
#include "task.hpp"

class Blink
{
public:

    static void Run(void *p)
    {
        ((Blink*)p)->Loop();
    }

    void Loop()
    {
        // Initialize
        GpioOutput blinker(GPIO_NUM_23);
        
        while (1)
        {
            blinker.Set();
            TASK_DELAY(1000);
    
            blinker.Clear();
            TASK_DELAY(1000);

            ESP_LOGI("BlinkTask", "Blink!");
        }        
    }
};