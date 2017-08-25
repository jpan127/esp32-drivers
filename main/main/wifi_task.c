#include "wifi_station.h"

void wifi_task(void *p)
{
    // Initialize as DHCP station
    wifi_dhcp_client_init();

    // Main loop
    while (1)
    {
        
        // Wait a little bit
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}