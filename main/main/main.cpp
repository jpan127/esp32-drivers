#include "blink.hpp"
#include "MotorDriverTask.cpp"

// Link app_main
extern "C" { void app_main(); }

void app_main()
{
    Blink *blink = new Blink;
    xTaskCreate(Blink::Run, "blink_task", 2048, (void*)blink, 5, NULL);

    xTaskCreate(&MotorDriverTask, "MotorDriverTask", 8196, NULL, 10, NULL);
}