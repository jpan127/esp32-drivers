#include "Wifi.hpp"

Wifi::Wifi(wifi_mode_t mode)
{
    Mode = mode;
    InitConfigDefault(mode);
    tcpip_adapter_init();    
}

Wifi::~Wifi()
{
    delete IP;
    delete Gateway;
    delete Netmask;
}

// Initialize wifi based on default configuration
void Wifi::InitConfigDefault(wifi_mode_t mode)
{   
    // Initialize first with default configuration
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
}

// Callback function that handles when wifi events happen
void Wifi::GeneralEventHandler(system_event_t *event)
{
    static const char* TAG = "Wifi::GeneralEventHandler";

    switch (event->event_id)
    {
        // Finished scanning for AP
        case SYSTEM_EVENT_SCAN_DONE:
            ESP_LOGI(TAG, "SYSTEM_EVENT_SCAN_DONE");
            break;
        // Started being a station
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            break;
        // Stopped being a station
        case SYSTEM_EVENT_STA_STOP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_STOP");
            break;
        // Connected to an AP as a station
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED");
            break;
        // Disconnected from AP as a station
        case SYSTEM_EVENT_STA_DISCONNECTED:
            // Should close all sockets
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            break;
        // Authentication mode has changed
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_AUTHMODE_CHANGE");
            break;
        // Got an IP address from AP as a station
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            break;
        // Started being an AP
        case SYSTEM_EVENT_AP_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_START");
            break;
        // Stopped being an AP
        case SYSTEM_EVENT_AP_STOP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STOP");
            break;
        // A station connected to us as an AP
        case SYSTEM_EVENT_AP_STACONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STACONNECTED");
            break;
        // A station disconnected from us as an AP
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            // Close socket related to station
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STADISCONNECTED, Code: %i", event->event_info.disconnected.reason);
            break;
        // Undefined event handling
        default:
            ESP_LOGE(TAG, "Unexpected Event: %d", event->event_id);
            break;
    }
}

void Wifi::PrintInfo()
{
    // Mode
    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    switch (mode)
    {
        case WIFI_MODE_NULL:    printf("Mode: WIFI_MODE_NULL\n");     break;
        case WIFI_MODE_STA:     printf("Mode: WIFI_MODE_STA\n");      break;
        case WIFI_MODE_AP:      printf("Mode: WIFI_MODE_AP\n");       break;
        case WIFI_MODE_APSTA:   printf("Mode: WIFI_MODE_APSTA\n");    break;
        case WIFI_MODE_MAX:     printf("Mode: I_MODE_MAX\n");         break;
    }

    // Power Save
    wifi_ps_type_t ps;
    ESP_ERROR_CHECK(esp_wifi_get_ps(&ps));
    switch (ps)
    {
        case WIFI_PS_NONE:
            printf("Power Save: WIFI_PS_NONE\n");
            break;
        case WIFI_PS_MODEM:
            printf("Power Save: WIFI_PS_MODEM\n");
            break;
    }
    
    // Bandwidth
    wifi_bandwidth_t bwidth;
    ESP_ERROR_CHECK(esp_wifi_get_bandwidth(ESP_IF_WIFI_STA, &bwidth));
    switch (bwidth)
    {
        case WIFI_BW_HT20:
            printf("STA Bandwidth: WIFI_BW_HT20\n");
            break;
        case WIFI_BW_HT40:
            printf("STA Bandwidth: WIFI_BW_HT40\n");
            break;
    }

    ESP_ERROR_CHECK(esp_wifi_get_bandwidth(ESP_IF_WIFI_AP, &bwidth));
    switch (bwidth)
    {
        case WIFI_BW_HT20:
            printf("AP Bandwidth: WIFI_BW_HT20\n");
            break;
        case WIFI_BW_HT40:
            printf("AP Bandwidth: WIFI_BW_HT40\n");
            break;
    }

    // Get station configuration
    wifi_config_t config;
    ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_STA, &config));

    printf("STA Configuration: SSID      : %s\n", config.sta.ssid);
    printf("STA Configuration: PASS      : %s\n", config.sta.password);
    printf("STA Configuration: BSSID_SET : %i\n", config.sta.bssid_set);
    printf("STA Configuration: BSSID     : %s\n", config.sta.bssid);
    printf("STA Configuration: CHANNEL   : %i\n", config.sta.channel);
    printf("STA Configuration: SSID      : %s\n", config.sta.ssid);
    printf("STA Configuration: PASS      : %s\n", config.sta.password);

    // Get AP configuration
    ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_AP, &config));

    printf("AP Configuration: CHANNEL   : %i\n", config.ap.channel);
    printf("AP Configuration: SSID_HIDD : %i\n", config.ap.ssid_hidden);
    printf("AP Configuration: MAX_CONN  : %i\n", config.ap.max_connection);
    printf("AP Configuration: BEACON_ITV: %i\n", config.ap.beacon_interval);
    
    switch (config.ap.authmode)
    {
        case WIFI_AUTH_OPEN:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_OPEN\n");
            break;
        case WIFI_AUTH_WEP:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_WEP\n");
            break;
        case WIFI_AUTH_WPA_PSK:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_WPA_PSK\n");
            break;
        case WIFI_AUTH_WPA2_PSK:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_WPA2_PSK\n");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_WPA_WPA2_PSK\n");
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_WPA2_ENTERPRISE\n");
            break;
        case WIFI_AUTH_MAX:
            printf("AP Configuration: AUTHMODE  : WIFI_AUTH_MAX\n");
            break;
    }
}

void Wifi::SetIpInfo(wifi_mode_t mode, const char *ip, const char *gw, const char *nm)
{
    tcpip_adapter_ip_info_t ip_info;

    inet_pton(AF_INET, ip,  &ip_info.ip);
    inet_pton(AF_INET, gw,  &ip_info.gw);
    inet_pton(AF_INET, nm,  &ip_info.netmask);
    
    switch (mode)
    {
        case WIFI_MODE_STA:
            tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
            break;
        case WIFI_MODE_AP:
            tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
            break;
        default:
            break;
    }
}

char* AuthmodeToString(wifi_auth_mode_t mode)
{
    switch (mode)
    {
        case WIFI_AUTH_OPEN:            return (char *)"WIFI_AUTH_OPEN";
        case WIFI_AUTH_WEP:             return (char *)"WIFI_AUTH_WEP";
        case WIFI_AUTH_WPA_PSK:         return (char *)"WIFI_AUTH_WPA_PSK";
        case WIFI_AUTH_WPA2_PSK:        return (char *)"WIFI_AUTH_WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:    return (char *)"WIFI_AUTH_WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE: return (char *)"WIFI_AUTH_WPA2_ENTERPRISE";
        case WIFI_AUTH_MAX:             return (char *)"WIFI_AUTH_MAX";
    }

    return NULL;
}

char* SecondChannelToString(wifi_second_chan_t second)
{
    switch (second)
    {
        case WIFI_SECOND_CHAN_NONE:     return (char *)"WIFI_SECOND_CHAN_NONE";
        case WIFI_SECOND_CHAN_ABOVE:    return (char *)"WIFI_SECOND_CHAN_ABOVE";
        case WIFI_SECOND_CHAN_BELOW:    return (char *)"WIFI_SECOND_CHAN_BELOW";
    }

    return NULL;
}