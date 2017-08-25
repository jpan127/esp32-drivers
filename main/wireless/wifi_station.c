#include "wifi.h"

// UDP Port 67 is for servers
// UDP Port 68 is for clients

// [STATION] Sets a new configuration
void wifi_set_sta_config(uint8_t ssid[32], uint8_t password[64], bool bssid_set)
{
    wifi_config_t config = {
        .sta = {
            .ssid       = ssid,
            .password   = password,
            .bssid_set  = bssid_set
        }
    };

    esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t *)&config);
}

// [STATION] Initialize DHCP client to connect to the DHCP server at the AP
void wifi_dhcp_client_init()
{
    ESP_LOGI("wifi_dhcp_client_init", "Starting DHCP client initialization.");

    nvs_flash_init();
    tcpip_adapter_init();
    // Stop if AP
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_AP);

    // Set IP info
    tcpip_adapter_ip_info_t *ip_info;
    wifi_set_up_info(ip_info, "192.168.1.99", "192.168.1.1", "255.255.255.0");

    // Set up event handler callback function
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));

    // Normal setup
    wifi_initialize();
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Set configuration
    wifi_set_sta_config(ssid, pass, bssid_set);
    // Start wifi
    WIFI_START();
    // Connect
    WIFI_CONNECT();

    ESP_LOGI("wifi_dhcp_client_init", "Finished DHCP client initialization.");
}

// [STATION] Set ip info using strings
void wifi_set_ip_info(tcpip_adapter_ip_info_t &ip_info, char *ip, char *gw, char *nm)
{
    inet_pton(AF_INET, ip,  &ip_info.ip);
    inet_pton(AF_INET, gw,  &ip_info.gw);
    inet_pton(AF_INET, nm,  &ip_info.netmask);
    
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
}

// [STATION] Set ip info using numbers
// Not sure if this overloaded function is useful
void wifi_set_ip_info(tcpip_adapter_ip_info_t &ip_info, char ip[4], char gw[4], char nm[4])
{
    inet_pton(&ip_info.ip,      ip[0], ip[1], ip[2], ip[3]);
    inet_pton(&ip_info.gw,      gw[0], gw[1], gw[2], gw[3]);
    inet_pton(&ip_info.netmask, nm[0], nm[1], nm[2], nm[3]);
    
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
}