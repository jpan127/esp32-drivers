#include <nvs_flash.h>
// #include "BlinkTask.hpp"
// #include "MotorDriverTask.cpp"
// #include "UdpServerTask.hpp"
#include "UdpClientTask.hpp"
// #include "WifiScanTask.hpp"
// #include "TcpServerTask.hpp"
#include "TemperatureReadTask.hpp"
#include "deep_sleep.hpp"

static void *EMPTY;

// Link app_main
extern "C" { void app_main(); }

void app_main()
{
    // Initialize flash
    ESP_ERROR_CHECK(nvs_flash_init());

    UdpClientTask udp_client_task("UdpClientTask", 8196);
    udp_client_task.Run(EMPTY);

    // UdpServerTask udp_server_task("UdpServerTask", 8196);
    // udp_server_task.Run(EMPTY);

    // TcpServerTask tcp_server_task("TcpServerTask", 8196);
    // tcp_server_task.Run(EMPTY);

    // BlinkTask blink_task;
    // blink_task.Run(EMPTY);

    // xTaskCreate(&MotorDriverTask, "MotorDriverTask", 8196, NULL, 10, NULL);
    // xTaskCreate(&WifiScanTask, "WifiScanTask", 8196, NULL, 10, NULL);

    // enter_deep_sleep(MINUTE_TO_MICRO(10));
}