#include "UdpSocket.hpp"
#include "WifiStation.hpp"
#include "utilities.h"
#include "credentials.hpp"          // Saved ssid and password
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
extern const uint8_t *NETWORK_SSID;
extern const uint8_t *NETWORK_PASS;

// Access
class UdpServer : public WifiStation
{
public:

    // Constructor
    UdpServer();

    // Initialize wifi
    void Initialize();
        
    // Initializes socket
    void InitializeSocket();

    // Wait for packet
    void WaitForPacket(bool block=true);

private:

    // Socket set up as a server, that listens for requests
    // Named with 'm' for member because ServerSocket looks too much like a class :(
    UdpSocket mServerSocket;
};