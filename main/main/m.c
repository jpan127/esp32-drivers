// #include "blink.h"
// #include "wifi_task.c"

// // Main function
// void app_main()
// {
//     // Initialize these in app_main instead of tasks
//     ESP_ERROR_CHECK(nvs_flash_init());
//     tcpip_adapter_init();

//     xTaskCreate(&blink_task,      "blink_task",       512,    NULL, 5, NULL);
//     xTaskCreate(&wifi_scan_task,  "wifi_scan_Task",   2048,   NULL, 5, NULL);
// }