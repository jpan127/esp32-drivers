#pragma once
#include "wifi.h"

/********************************************************************************************
    Initialization:
        tcpip_adapter_init() to create an lwIP core task and initialize.
        esp_event_loop_init() to create an event task an initialize callback.
        esp_wifi_init() to create a wifi driver task and initialize.
    Configuration:
        esp_wifi_set_mode(WIFI_MODE_STA) to set mode as station.
        esp_wifi_set_config() to set basic configuration details (ssid, pass).
    Start:
        esp_wifi_start() to start the driver.
        esp_wifi_connect() to connect to AP.
    Connect:
        Start scanning and wait for SYSTEM_EVENT_STA_CONNECTED.
        If not found, SYSTEM_EVENT_STA_DISCONNECTED, need to handle reconnection.
    Got IP:
        SYSTEM_EVENT_STA_GOT_IP
        Can start socket-related work after getting IP.
    Disconnect:
        Need to disconnect all sockets.
        Re-call esp_wifi_connect().
    IP Change:
        SYSTEM_EVENT_STA_GOT_IP
        Need to close all sockets, then re-initialize them.
    Deinitialization:
        esp_wifi_disconnect() to disconnect wifi.
        esp_wifi_stop() to stop driver.
        esp_wifi_deinit() to unload driver.
********************************************************************************************/

// [AP] Set a new configuration
void wifi_set_ap_config(uint8_t ssid[32], uint8_t password[64], 
    uint8_t ssid_len, uint8_t channel, wifi_auth_mode_t authmode, 
    uint8_t bssid_hidden, uint8_t max_connection, uint16_t beacon_interval);

// [AP] Get STATION info
void wifi_get_station_info();

// [AP] Initialize device as a DHCP server so connecting stations
// can auto assign IP addresses, learn subnet masks, gateways
void wifi_dhcp_server_init();