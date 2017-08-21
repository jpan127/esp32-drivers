#include "wifi.h"

/*  
 *  TODO:
 *  Split station and AP functions into separate sections
 */

// Initialize wifi first
void wifi_initialize()
{
    static const char* TAG = "wifi_task";
    
    // Initialize first with default configuration
    wifi_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    WIFI_INIT(&config);

    // Set custom configuration
    wifi_set_config("ssid", "password", false);

    // Print info
    wifi_print_info();
    
    // ...


    // When done setting up, start up wifi
    WIFI_START();
}

void wifi_task(void *p)
{
    // Main loop
    while (1)
    {
        
        // Wait a little bit
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

// [STATION] Initialize DHCP client to connect to the DHCP server at the AP
void wifi_dhcp_client_init()
{
    tcpip_adapter_init();
    tcpip_adapter_dhcpc_stop();

    // Set IP info
    tcpip_adapter_ip_info_t *ip_info;
    wifi_set_up_info(ip_info, "192.168.1.99", "192.168.1.1", "255.255.255.0");

    // Normal setup
    esp_wifi_init();
    esp_wifi_set_mode();
    esp_wifi_set_config();
    esp_wifi_config();
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

// Sets a new STATION configuration
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

// Sets a new ACCESS POINT configuration
void wifi_set_ap_config(uint8_t ssid[32], uint8_t password[64], 
    uint8_t ssid_len, uint8_t channel, wifi_auth_mode_t authmode, 
    uint8_t bssid_hidden, uint8_t max_connection, uint16_t beacon_interval)
{
    wifi_config_t config = {
        .ap = {
            .ssid       = ssid,
            .password   = password,
            .ssid_len   = ssid_len,
            .channel    = channel,
            .authmode   = authmode,
            .bssid_hidden    = bssid_hidden,
            .max_connection  = max_connection,
            .beacon_interval = beacon_interval
        }
    };

    esp_wifi_set_config(WIFI_IF_AP, (wifi_config_t *)&config);
}

// Get STATION info as an ACCESS POINT
void wifi_get_station_info()
{
    // Not sure if there is a way to get number of stations

    wifi_sta_list *station_list;
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list);

    while (station_list)
    {
        ESP_LOGI("Station IP: %d.%d.%d.%d", IP2STR(&(station_list->ip)));
        // Traverse link list
        station_list = STAILQ_NEXT(station_list, NEXT);
    }
}

// Callback function that handles when wifi events happen
static void wifi_event_handler(system_event_t *event)
{
    static const char* TAG = "wifi_event_handler";

    switch (event->event_id)
    {
        // Finished scanning for AP
        case: SYSTEM_EVENT_SCAN_DONE:
            ESP_LOGI(TAG, "SYSTEM_EVENT_SCAN_DONE");
            break;
        // Started being a station
        case: SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            break;
        // Stopped being a station
        case: SYSTEM_EVENT_STA_STOP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_STOP");
            break;
        // Connected to an AP as a station
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED");
            break;
        // Disconnected from AP as a station
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            break;
        // Authentication mode has changed
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_AUTHMODE_CHANGE");
            break;
        // Got an IP address from AP as a station
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP, IP: ", IP2STR(event->event_info.got_ip.ip_info.ip));
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
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STADISCONNECTED, Code: %i", event->event_info.disconnected.reason);
            break;
        // Received a probe request as an AP
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_PROBEREQRECVED");
            break;
        // Undefined event handling
        default:
            ESP_LOGE(TAG, "Unexpected Event: %d", event->event_id);
            break;
    }
}

void wifi_print_info()
{
    // Mode
    wifi_mode_t *mode;
    WIFI_GET_MODE(mode);
    switch (mode)
    {
        case WIFI_MODE_NULL:    ESP_LOGI(TAG, "WIFI_MODE_NULL");     break;
        case WIFI_MODE_STA:     ESP_LOGI(TAG, "WIFI_MODE_STA");      break;
        case WIFI_MODE_AP:      ESP_LOGI(TAG, "WIFI_MODE_AP");       break;
        case WIFI_MODE_APSTA:   ESP_LOGI(TAG, "WIFI_MODE_APSTA");    break;
        case WIFI_MODE_MAX:     ESP_LOGI(TAG, "WIFI_MODE_MAX");      break;
    }

    // Power Save
    wifi_ps_type_t *ps;
    WIFI_GET_PS(ps);
    switch (ps)
    {
        case WIFI_PS_NONE:
            ESP_LOGI("Power Save", "WIFI_PS_NONE");
            break;
        case WIFI_PS_MODEM:
            ESP_LOGI("Power Save", "WIFI_PS_MODEM");
            break;
    }
    
    // Bandwidth
    wifi_interface_t iface;
    wifi_bandwidth_t *bwidth;

    iface = ESP_IF_WIFI_STA;
    WIFI_GET_BANDWIDTH(iface, bwidth);
    switch (bwidth)
    {
        case WIFI_BW_HT20:
            ESP_LOGI("STA Bandwidth", "WIFI_BW_HT20");
            break;
        case WIFI_BW_HT40:
            ESP_LOGI("STA Bandwidth", "WIFI_BW_HT40");
            break;
    }

    iface = ESP_IF_WIFI_AP;
    WIFI_GET_BANDWIDTH(iface, bwidth);
    switch (bwidth)
    {
        case WIFI_BW_HT20:
            ESP_LOGI("AP Bandwidth", "WIFI_BW_HT20");
            break;
        case WIFI_BW_HT40:
            ESP_LOGI("AP Bandwidth", "WIFI_BW_HT40");
            break;
    }

    // Configuration
    wifi_config_t *config;

    iface = ESP_IF_WIFI_STA;
    WIFI_GET_CONFIG(iface, config);

    ESP_LOGI("STA Configuration", "SSID      : %i", config.sta.ssid);
    ESP_LOGI("STA Configuration", "PASS      : %i", config.sta.password);
    ESP_LOGI("STA Configuration", "BSSID_SET : %i", config.sta.bssid_set);
    ESP_LOGI("STA Configuration", "BSSID     : %i", config.sta.bssid);
    ESP_LOGI("STA Configuration", "CHANNEL   : %i", config.sta.channel);

    iface = ESP_IF_WIFI_AP;
    WIFI_GET_CONFIG(iface, config);
    
    ESP_LOGI("AP Configuration", "SSID      : %i", config.ap.ssid);
    ESP_LOGI("AP Configuration", "PASS      : %i", config.ap.password);
    ESP_LOGI("AP Configuration", "SSID_LEN  : %i", config.ap.ssid_len);
    ESP_LOGI("AP Configuration", "CHANNEL   : %i", config.ap.channel);
    ESP_LOGI("AP Configuration", "SSID_HIDD : %i", config.ap.channel);
    ESP_LOGI("AP Configuration", "MAX_CONN  : %i", config.ap.channel);
    ESP_LOGI("AP Configuration", "BEACON_ITV: %i", config.ap.channel);
    
    switch (config.ap.authmode)
    {
        case WIFI_AUTH_OPEN:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_OPEN");
            break;
        case WIFI_AUTH_WEP:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_WEP");
            break;
        case WIFI_AUTH_WPA_PSK:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_WPA_PSK");
            break;
        case WIFI_AUTH_WPA2_PSK:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_WPA2_PSK");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_WPA_WPA2_PSK");
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_WPA2_ENTERPRISE");
            break;
        case WIFI_AUTH_MAX:
            ESP_LOGI("AP Configuration", "AUTHMODE  : WIFI_AUTH_MAX");
            break;
    }

    // Deallocate dynamic memory
    free(mode);
    free(ps);    
    free(bwidth);
    free(config);
}