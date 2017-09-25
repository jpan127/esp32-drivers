#include "UdpClient.hpp"

static EventGroupHandle_t EventGroup;

static void EventHandler(void *ctx, system_event_t *event)
{
    static const char *TAG = "UdpClient::EventHandler";

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

///////////////////////////////////////////////////////////////////////////////////////////////////

UdpClient::UdpClient(port_t Port) : WifiStation(), mClientSocket(Port)
{
    EventGroup = xEventGroupCreate();
}

void UdpClient::Initialize()
{
    // Initialize TCPIP
    tcpip_adapter_init();
    // Don't run a DHCP server
    tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
    // Set static ip, gw, sn
    SetStationIpInfo(DEVICE_IP, DEVICE_GW, DEVICE_SN);

    // Set up station configuration
    SetConfig(NETWORK_SSID, NETWORK_PASS);
    ESP_LOGI("UdpClient::Initialize", "Station configuration initialized SSID: %s", NETWORK_SSID);

    // Set event callback
    ESP_ERROR_CHECK(esp_event_loop_init((system_event_cb_t)EventHandler, NULL));

    // Start
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("UdpClient::Initialize", "Starting wifi...");

    // Connect
    ESP_ERROR_CHECK(esp_wifi_connect());
    ESP_LOGI("UdpClient::Initialize", "Connecting wifi...");
}

void UdpClient::InitializeSocket()
{
    // Wait for wifi connection before creating sockets
    xEventGroupWaitBits(EventGroup, BIT_CONNECTED, pdTRUE, pdTRUE, FIVE_MIN / portTICK_PERIOD_MS);
    // Initialize socket
    mClientSocket.UdpCreateSocket();
    ESP_LOGI("UdpClient::InitializeSocket", "UDP Socket created.");
}

void UdpClient::SendPacket(char *packet, int packet_length, port_t port, char *server_ip)
{
    mClientSocket.UdpSend(packet, packet_length, port, server_ip);
}