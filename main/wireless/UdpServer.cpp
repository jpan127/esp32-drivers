#include "UdpServer.hpp"

UdpServer::UdpServer() : WifiStation(), mServerSocket(PORT)
{
    // Create an event group
    EventGroup = xEventGroupCreate();
}

static void EventHandler(void *ctx, system_event_t *event)
{
    static const char *TAG = "UdpServer::EventHandler";

    switch (event->event_id)
    {
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
}

void UdpServer::Initialize()
{
    // Initialize TCPIP
    tcpip_adapter_init();
    // Don't run a DHCP server
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
    // Set static ip, gw, sn
    SetStationIpInfo(DEVICE_IP, DEVICE_GW, DEVICE_SN);

    // Set up station configuration
    SetConfig(NETWORK_SSID, NETWORK_PASS);
    ESP_LOGI("UdpServer::Initialize", "Station configuration initialized SSID: %s", NETWORK_SSID);

    // Set event callback
    ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)EventHandler, NULL));

    // Start
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("UdpServer::Initialize", "Starting wifi...");

    // Connect
    ESP_ERROR_CHECK(esp_wifi_connect());
    ESP_LOGI("UdpServer::Initialize", "Connecting wifi...");
}

void UdpServer::InitializeSocket()
{
    // Wait for wifi connection before creating sockets
    xEventGroupWaitBits(EventGroup, BIT_CONNECTED, pdTRUE, pdTRUE, FIVE_MIN / portTICK_PERIOD_MS);
    // Initialize socket
    mServerSocket.UdpCreateSocket();
    ESP_LOGI("UdpServer::InitializeSocket", "UDP Socket created.");
    mServerSocket.Bind();
    ESP_LOGI("UdpServer::InitializeSocket", "UDP Socket binded.");
}

void UdpServer::WaitForPacket(bool block)
{
    ESP_LOGI("UdpServer::WaitForPacket", "Waiting for packet...");
    mServerSocket.UdpReceive(block);
}