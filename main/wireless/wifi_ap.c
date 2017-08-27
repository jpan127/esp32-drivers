#include "wifi_ap.h"

// [AP] Initialize device as a DHCP server so connecting stations
// can auto assign IP addresses, learn subnet masks, gateways
void wifi_dhcp_server_init()
{
    /*
        wifi_softap_dhcps_start();
        wifi_softap_dhcps_stop();
        wifi_softap_dhcps_status();
        // Default range of DHCP server is 192.168.4.1 upwards
        tcpip_adapter_get_ip_info();
    */
}

// [AP] Sets a new configuration
void wifi_set_ap_config(uint8_t ssid[32], uint8_t password[64], 
    uint8_t ssid_len, uint8_t channel, wifi_auth_mode_t authmode, 
    uint8_t ssid_hidden, uint8_t max_connection, uint16_t beacon_interval)
{
    wifi_config_t config = {
        .ap = {
            .ssid_len   = ssid_len,
            .channel    = channel,
            .authmode   = authmode,
            .ssid_hidden     = ssid_hidden,
            .max_connection  = max_connection,
            .beacon_interval = beacon_interval
        }
    };

    // Could not get it to compile in initializer, and could not link <cstring> for memcpy...
    for (int i=0; i<32; i++) config.ap.ssid[i]      = ssid[i];
    for (int i=0; i<64; i++) config.ap.password[i]  = password[i];

    esp_wifi_set_config(WIFI_IF_AP, (wifi_config_t *)&config);
}

// [AP] Get STATION info
void wifi_get_station_info()
{
    wifi_sta_list_t station_list;
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&station_list));

    for (int i=0; i<station_list.num; i++) {
        ESP_LOGI("wifi_get_station_info", "%s", station_list.sta[i].mac);
    }
}