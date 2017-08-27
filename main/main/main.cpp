#include "wifi.h"
#include "pwm.hpp"

// Link app_main
extern "C" { void app_main(); }

void hello_task(void *p)
{
    while (1)
    {
        printf("Hello");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

class Hello
{
public:

    Hello() {}

    void CreateTask(int stack_size, uint8_t priority)
    {
        // Just for now
        const uint8_t max_priority  = 10;
        const int max_stack_size    = 2048;

        if (stack_size > max_stack_size)    stack_size  = max_stack_size;
        if (priority > max_priority)        priority    = max_priority;

        xTaskCreate(&hello_task, "hello_task", stack_size, NULL, priority, NULL);
    }
};

void app_main()
{
    Hello hello;
    hello.CreateTask(2048, 5);
}