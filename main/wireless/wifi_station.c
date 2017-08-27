#include "wifi_station.h"

// UDP Port 67 is for servers
// UDP Port 68 is for clients

// [STATION] Sets a new configuration
void wifi_set_sta_config(uint8_t ssid[32], uint8_t password[64], bool bssid_set)
{
    wifi_config_t config;
    config.sta.bssid_set = bssid_set;
    // Could not get it to compile in initializer, and could not link <cstring> for memcpy...
    for (int i=0; i<32; i++) config.ap.ssid[i]      = ssid[i];
    for (int i=0; i<64; i++) config.ap.password[i]  = password[i];

    esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)&config);
}

// [STATION] Set ip info using strings
void wifi_set_ip_info(tcpip_adapter_ip_info_t *ip_info, char *ip, char *gw, char *nm)
{
    inet_pton(AF_INET, ip,  &ip_info->ip);
    inet_pton(AF_INET, gw,  &ip_info->gw);
    inet_pton(AF_INET, nm,  &ip_info->netmask);
    
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, ip_info);
}

// [STATION] Initialize DHCP client to connect to the DHCP server at the AP
void wifi_dhcp_client_init(uint8_t ssid[32], uint8_t pass[64], bool bssid_set)
{
    ESP_LOGI("wifi_dhcp_client_init", "Starting DHCP client initialization.");

    // Stop if AP
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_AP);

    // Set IP info
    tcpip_adapter_ip_info_t ip_info;
    wifi_set_ip_info(&ip_info, "192.168.1.99", "192.168.1.1", "255.255.255.0");

    // Set up event handler callback function
    ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)wifi_event_handler, NULL));

    // Normal setup
    wifi_config_default();
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Set configuration
    wifi_set_sta_config(ssid, pass, bssid_set);
    // Start wifi
    ESP_ERROR_CHECK(esp_wifi_start());
    // Connect
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI("wifi_dhcp_client_init", "Finished DHCP client initialization.");
}

void wifi_sta_scan_all()
{
    // // Set country code to set channel scan range
    // ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_US));

    // Configure
    wifi_scan_config_t scan_config;
    scan_config.ssid        = 0;
    scan_config.bssid       = 0;
    scan_config.channel     = 0;
    scan_config.scan_type   = WIFI_SCAN_TYPE_ACTIVE;
    scan_config.show_hidden = true;
    // ??? not sure what's a good time
    scan_config.scan_time.active.min = 100;
    scan_config.scan_time.active.max = 1000;

    // Start scan
    esp_wifi_scan_start(&scan_config, true);
}

void wifi_sta_scan_specific(uint8_t ssid[32])
{
    // // Set country code to set channel scan range
    // ESP_ERROR_CHECK(esp_wifi_set_country(WIFI_COUNTRY_US));

    // Configure
    wifi_scan_config_t scan_config;
    strncpy((char *)&scan_config.ssid, (char *)&ssid, 32);
    scan_config.bssid       = 0;
    scan_config.channel     = 0;
    scan_config.scan_type   = WIFI_SCAN_TYPE_ACTIVE;
    scan_config.show_hidden = true;
    // ??? not sure what's a good time
    scan_config.scan_time.active.min = 100;
    scan_config.scan_time.active.max = 1000;

    // Start scan
    esp_wifi_scan_start(&scan_config, true);
}