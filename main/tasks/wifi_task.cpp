#include "wifi_station.hpp"
#include "wifi_ap.hpp"
#include "utilities.h"

// static uint8_t *ssid = (uint8_t *)("JP Home");
// static uint8_t *pass = (uint8_t *)("chipan123");

esp_err_t wifi_scan_event_handler(void *ctx, system_event_t *event)
{
    static const char *TAG = "wifi_scan_event_handler";

    if (event->event_id == SYSTEM_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "SYSTEM_EVENT_SCAN_DONE");
        // Need to get AP records and free the memory allocated for it
        uint16_t num = 0;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&num));

        ESP_LOGI("", "-------------------------------------------------------------");
        ESP_LOGI(TAG, "AP number           : %i", num);
        ESP_LOGI(TAG, "Number of APs found : %i", event->event_info.scan_done.number);

        // Allocate array of ap_records
        wifi_ap_record_t *ap_records = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * num);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&num, ap_records));

        for (int i=0; i<num; i++) 
        {
            // Do not need to be freed because string literal
            char *second    = wifi_second_to_string(ap_records[i].second);
            char *auth_mode = wifi_authmode_to_string(ap_records[i].authmode);

            ESP_LOGI(TAG, "[AP %i] %1s | %s | %s",
                                                    i,
                                                    ap_records[i].ssid, 
                                                    // ap_records[i].bssid, 
                                                    // ap_records[i].primary,
                                                    // ap_records[i].rssi, 
                                                    second,
                                                    auth_mode);
        }

        ESP_LOGI("", "-------------------------------------------------------------");

        free(ap_records);
    }

    return ESP_OK;
}

void wifi_scan_task(void *p)
{
    nvs_flash_init();
    tcpip_adapter_init();

    // Register an event handler
    ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)wifi_scan_event_handler, NULL));

    // Initialize default configuration
    wifi_config_default(WIFI_MODE_STA);
    // Set country code to set channel scan range
    ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_US));
    
    // Start wifi
    ESP_ERROR_CHECK(esp_wifi_start());

    // Main loop
    while (1)
    {
        // Scan all
        wifi_sta_scan_all();

        // Every 5 seconds
        TASK_DELAY(5000);
    }
}