#include "BlinkTask.hpp"
#include "MotorDriverTask.cpp"
#include "UdpServerTask.hpp"
#include "WifiScanTask.hpp"

static void *EMPTY;

// Link app_main
extern "C" { void app_main(); }

void app_main()
{
    // Initialize flash
    ESP_ERROR_CHECK(nvs_flash_init());
        
    // BlinkTask blink_task;
    // blink_task.Run(EMPTY);


    UdpServerTask udp_server_task("UdpServerTask", 8196);
    udp_server_task.Run(EMPTY);



    // xTaskCreate(&MotorDriverTask, "MotorDriverTask", 8196, NULL, 10, NULL);
    // xTaskCreate(&WifiScanTask, "WifiScanTask", 8196, NULL, 10, NULL);    
}