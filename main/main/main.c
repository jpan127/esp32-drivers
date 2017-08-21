#include "blink.h"
#include "wifi.h"

// extern void blink_task(void *pvParameter);
// extern void print_ip_task(void *p);
// extern void app_main();

// Main function
void app_main()
{
    xTaskCreate(&blink_task,    "blink_task",       512,    NULL, 5, NULL);
    xTaskCreate(&print_ip_task, "print_ip_task",    2048,   NULL, 5, NULL);
}
