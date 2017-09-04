#pragma once
#include "WifiStation.hpp"
#include "utilities.h"

static void WifiScanTask_EventHandler(void *ctx, system_event_t *event)
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
            char *second    = SecondChannelToString(ap_records[i].second);
            char *auth_mode = AuthmodeToString(ap_records[i].authmode);

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
}

class WifiScanTask : public Task
{
public:
    
    WifiScanTask(std::string task_name="WifiScanTask", 
                                            uint16_t stack_size=2048,
                                            uint8_t priority=5) : 
                                            Task(task_name, stack_size, priority),
                                            Station()
    {
        /* EMPTY */
    }

    esp_err_t EventHandler(void *ctx, system_event_t *event)
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
                char *second    = SecondChannelToString(ap_records[i].second);
                char *auth_mode = AuthmodeToString(ap_records[i].authmode);
    
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

    void Run(void *p)
    {
        // Register an event handler
        ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)WifiScanTask_EventHandler, NULL));
        
        // Start wifi
        ESP_ERROR_CHECK(esp_wifi_start());
    
        // Main loop
        while (1)
        {
            // Scan all
            Station.ScanAll();
    
            // Every 5 seconds
            TASK_DELAY(5000);
        }
    }

private:

    WifiStation Station;
};