#pragma once
#include "Wifi.hpp"

class WifiStation : public Wifi
{
public:
    
    WifiStation();

    // Sets a new configuration
    void SetConfig(const uint8_t ssid[32], const uint8_t password[64], const bool bssid_set=0);

    // Scans all channels for all APs
    void ScanAll();

    // Scans for a specific AP across all channels
    void ScanSpecific(uint8_t ssid[32]);

    void SetStationIpInfo(const char *ip, const char *gw, const char *nm);

private:

};

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