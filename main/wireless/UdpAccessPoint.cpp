#include "socket.hpp"
#include "wifi_ap.hpp"
#include "wifi_station.hpp"
#include "utilities.h"
#include <freertos/event_groups.h>

#define CLIENT_IP           ("192.168.1.229")
#define PORT                (4222)
#define DEVICE_IP           ("192.168.1.250")
#define DEVICE_GW           ("192.168.1.1")
#define DEVICE_SN           ("255.255.255.0")
#define BIT_START           (1 << 0)
#define BIT_STOP            (1 << 1)
#define BIT_CONNECTED       (1 << 2)
#define BIT_DISCONNECTED    (1 << 3)
#define FIVE_MIN            (5*60*1000)

static EventGroupHandle_t EventGroup;
// static uint8_t *NETWORK_SSID = (uint8_t *)"JP Home";
// static uint8_t *NETWORK_PASS = (uint8_t *)"chipan123";

class UdpAccessPoint
{
public:

    // Constructor
    UdpAccessPoint();

    // Initialize wifi
    void Initialize();
        
    // Initializes socket
    void InitializeSocket();

    // Wait for packet
    void WaitForPacket();

private:

    Socket UdpSocket;
};

UdpAccessPoint::UdpAccessPoint() : UdpSocket(PORT)
{
    // Create an event group
    EventGroup = xEventGroupCreate();
}

esp_err_t EventHandler(void *ctx, system_event_t *event)
{
    static const char *TAG = "UdpAccessPoint::EventHandler";

    switch (event->event_id)
    {
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
            ESP_LOGI(TAG, "SYSTEM_EVENT_AP_STADISCONNECTED, Error: %i", 
                                                    event->event_info.disconnected.reason);
            // Close socket related to station
            break;
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            xEventGroupSetBits(EventGroup, BIT_START);
            break;
        case SYSTEM_EVENT_STA_STOP:
            xEventGroupSetBits(EventGroup, BIT_STOP);
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_STOP");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED");
            xEventGroupSetBits(EventGroup, BIT_CONNECTED);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            xEventGroupSetBits(EventGroup, BIT_DISCONNECTED);
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED Error: %i",
                                                    event->event_info.disconnected.reason);
            ESP_LOGI(TAG, "Reconnecting in 5...");
            TASK_DELAY(5000);
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(TAG, "Reconnecting...");
            break;
        case SYSTEM_EVENT_WIFI_READY:
            ESP_LOGI(TAG, "SYSTEM_EVENT_WIFI_READY");
            break;
        default:
            break;
    }

    return ESP_OK;
}

void UdpAccessPoint::Initialize()
{
    nvs_flash_init();
    // Initialize TCPIP
    tcpip_adapter_init();
    // Don't run a DHCP server
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
    // Set static ip, gw, sn
    wifi_set_ip_info(DEVICE_IP, DEVICE_GW, DEVICE_SN);

    // Set up wifi configuration
    wifi_config_default(WIFI_MODE_STA);
    ESP_LOGI("UdpAccessPoint::Initialize", "Default wifi configuration initialized.");

    // Set up AP configuration
    // wifi_set_sta_config(NETWORK_SSID, NETWORK_PASS);
    wifi_config_t station_config;
    uint8_t ssid[32] = "JP Home";
    uint8_t pass[64] = "chipan123";
    strncpy((char *)station_config.sta.ssid, (char *)ssid, 32);
    strncpy((char *)station_config.sta.password, (char *)pass, 64);
    station_config.sta.bssid_set = 0;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &station_config));    
    ESP_LOGI("UdpAccessPoint::Initialize", "Station configuration initialized SSID: %s", ssid);

    // Set event callback
    ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)(EventHandler), NULL));

    // Start
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("UdpAccessPoint::Initialize", "Starting wifi...");

    // Connect
    ESP_ERROR_CHECK(esp_wifi_connect());
    ESP_LOGI("UdpAccessPoint::Initialize", "Connecting wifi...");
}

void UdpAccessPoint::InitializeSocket()
{
    // Wait for wifi connection
    xEventGroupWaitBits(EventGroup, BIT_CONNECTED, pdTRUE, pdTRUE, FIVE_MIN / portTICK_PERIOD_MS);
    // Initialize socket
    UdpSocket.CreateSocket(true);
    ESP_LOGI("UdpAccessPoint::InitializeSocket", "UDP Socket created.");
    UdpSocket.Bind();
    ESP_LOGI("UdpAccessPoint::InitializeSocket", "UDP Socket binded.");
}

void UdpAccessPoint::WaitForPacket()
{
    ESP_LOGI("UdpAccessPoint::WaitForPacket", "Waiting for packet...");
    UdpSocket.Receive(true);
    ESP_LOGI("UdpAccessPoint::WaitForPacket", "Received packed.");
}