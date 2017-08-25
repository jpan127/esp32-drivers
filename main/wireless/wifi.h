#pragma once
#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
// #include <sdkconfig.h>
#include <tcpip_adapter.h>              // IPSTR , IP2STR
#include <esp_event.h>                  // system_event_t
#include <esp_wifi.h>                   // Wifi
#include <esp_err.h>                    // Error handling

/********************************************************************************************
*           Helper macros that wrap the esp_wifi functions with an error check              *
********************************************************************************************/

// Initialize wifi with a configuration
#define WIFI_INIT(config)                   (ESP_ERROR_CHECK(esp_wifi_init(config)))
// Set/Get current operating mode of wifi
#define WIFI_SET_MODE(mode)                 (ESP_ERROR_CHECK(esp_wifi_set_mode(mode)))
#define WIFI_GET_MODE(mode)                 (ESP_ERROR_CHECK(esp_wifi_get_mode(mode)))
// Set/Get current power save type
#define WIFI_SET_PS(type)                   (ESP_ERROR_CHECK(esp_wifi_set_ps(type)))
#define WIFI_GET_PS(type)                   (ESP_ERROR_CHECK(esp_wifi_get_ps(type)))
// Connect/Disconnect the device to/from the AP (access point)
#define WIFI_CONNECT()                      (ESP_ERROR_CHECK(esp_wifi_connect()))
#define WIFI_DISCONNECT()                   (ESP_ERROR_CHECK(esp_wifi_disconnect()))
// Scan all available APs (access points)
#define WIFI_SCAN(config, block)            (ESP_ERROR_CHECK(esp_wifi_scan_start(config, block)))
// Set/Get the bandwidth of the specified interface
#define WIFI_SET_BANDWIDTH(iface, bwidth)   (ESP_ERROR_CHECK(esp_wifi_set_bandwidth(iface, bwidth)))
#define WIFI_GET_BANDWIDTH(iface, bwidth)   (ESP_ERROR_CHECK(esp_wifi_get_bandwidth(iface, bwidth)))
// Set/Get configuration
#define WIFI_SET_CONFIG(iface, config)      (ESP_ERROR_CHECK(esp_wifi_set_config(iface, config)))
#define WIFI_GET_CONFIG(iface, config)      (ESP_ERROR_CHECK(esp_wifi_get_config(iface, config)))
// Set wifi configuration storage type
#define WIFI_SET_STORAGE(storage)           (ESP_ERROR_CHECK(esp_wifi_set_storage(storage)))
// Start wifi
#define WIFI_START()                        (ESP_ERROR_CHECK(esp_wifi_start()))
// Stop wifi
#define WIFI_STOP()                         (ESP_ERROR_CHECK(esp_wifi_stop()))


// Initialize wifi first
void wifi_initialize();

void wifi_task(void *p);

// [STATION] Initialize DHCP client to connect to the DHCP server at the AP
void wifi_dhcp_client_init();

// [STATION] Set ip info using strings
void wifi_set_ip_info(tcpip_adapter_ip_info_t &ip_info, char *ip, char *gw, char *nm);

// [STATION] Set ip info using numbers
// Not sure if this overloaded function is useful
void wifi_set_ip_info(tcpip_adapter_ip_info_t &ip_info, char ip[4], char gw[4], char nm[4]);

// [AP] Initialize device as a DHCP server so connecting stations
// can auto assign IP addresses, learn subnet masks, gateways
void wifi_dhcp_server_init();

// Sets a new STATION configuration
void wifi_set_sta_config(uint8_t ssid[32], uint8_t password[64], bool bssid_set);

// Sets a new ACCESS POINT configuration
void wifi_set_ap_config(uint8_t ssid[32], uint8_t password[64], 
    uint8_t ssid_len, uint8_t channel, wifi_auth_mode_t authmode, 
    uint8_t bssid_hidden, uint8_t max_connection, uint16_t beacon_interval);

// Get STATION info as an ACCESS POINT
void wifi_get_station_info();

// Callback function that handles when wifi events happen
void wifi_event_handler(system_event_t *event);

void wifi_print_info();