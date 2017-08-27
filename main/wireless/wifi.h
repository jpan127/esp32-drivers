#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
#include <tcpip_adapter.h>              // TCP/IP
#include <esp_event.h>                  // system_event_t
#include <esp_wifi.h>                   // Wifi
#include <esp_err.h>                    // Error handling
#include <esp_event_loop.h>             // Event loop handling

// Initialize wifi based on default configuration
void wifi_config_default();

// Callback function that handles when wifi events happen
void wifi_event_handler(system_event_t *event);

// Prints information about various wifi related data structures
void wifi_print_info();

// Returns auth mode in string
char* wifi_authmode_to_string(wifi_auth_mode_t mode);

// Returns scan second channel in string
char* wifi_second_to_string(wifi_second_chan_t second);

#ifdef __cplusplus
}
#endif