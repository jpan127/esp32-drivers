#pragma once
#include "wifi.h"
#include <string.h>

// [AP] Set a new configuration
//  Parameters:
//      authmode:           how station should authenticate
//      ssid_len:           specify length of ssid IF SSID IS NOT NULL TERMINATED
//      channel:            channel used for networking
//      ssid_hidden:        to broadcast ssid or not
//      max_connection:     max number of concurrent stations
//      becaon_interval:    no idea, just use default=100
void wifi_set_ap_config(uint8_t ssid[32], uint8_t password[64], 
                                            uint8_t ssid_len = 0, 
                                            wifi_auth_mode_t authmode=WIFI_AUTH_OPEN,
                                            uint8_t channel=0,
                                            uint8_t ssid_hidden=0, 
                                            uint8_t max_connection=4, 
                                            uint16_t beacon_interval=100);

// [AP] Get STATION info
void wifi_get_station_info();

// [AP] Initialize device as a DHCP server so connecting stations
// can auto assign IP addresses, learn subnet masks, gateways
void wifi_dhcp_server_init();