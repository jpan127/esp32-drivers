#pragma once
#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
#include <tcpip_adapter.h>              // TCP/IP
#include <esp_event.h>                  // system_event_t
#include <esp_wifi.h>                   // Wifi
#include <esp_err.h>                    // Error handling
#include <esp_event_loop.h>             // Event loop handling
#include "lwip/sockets.h"               // AF_INET
#include "nvs_flash.h"                  // nvs_flash_init()
#include <string.h>                     // strncpy

class Wifi
{
public:

    // Destructor
    ~Wifi();

    // Callback function that handles when wifi events happen
    void    GeneralEventHandler(system_event_t *event);

    // Prints information about various wifi related data structures
    void    PrintInfo();

    // Set TCPIP adapter info: ip, gateway, subnet
    void    SetIpInfo(wifi_mode_t mode, const char *ip, const char *gw, const char *nm);

protected:

    // Constructor
    Wifi(wifi_mode_t mode);
        
private:

    // Constructor calls this automatically
    // Initialize wifi based on default configuration
    void    InitConfigDefault(wifi_mode_t mode);
    
    wifi_mode_t     Mode;
    char            *IP;
    char            *Gateway;
    char            *Netmask;
};

// Returns auth mode in string
char*   AuthmodeToString(wifi_auth_mode_t mode);

// Returns scan second channel in string
char*   SecondChannelToString(wifi_second_chan_t second);
