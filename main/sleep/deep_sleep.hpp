#pragma once
#include <esp_wifi.h>
#include <esp_deep_sleep.h>

#define MINUTE_TO_MICRO(min) (min * 1000000UL)

void enter_deep_sleep(uint64_t microseconds)
{
    // Stop wifi
    // ESP_ERROR_CHECK(esp_wifi_stop());

    // Enable wakeup timer
    ESP_ERROR_CHECK(esp_deep_sleep_enable_timer_wakeup(microseconds));

    ESP_LOGI("Deep Sleep", "Entering deep sleep for %llu microseconds.", microseconds);
    // Start deep sleep
    esp_deep_sleep_start();    
}