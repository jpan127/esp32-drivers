#pragma once
#include "WifiStation.hpp"
#include "TcpSocket.hpp"
#include "utilities.h"
#include "credentials.hpp"          // Saved ssid and password
#include <freertos/event_groups.h>

#define CLIENT_IP           ("192.168.1.229")
#define TCP_DEFAULT_PORT    (4111)
#define DEVICE_IP           ("192.168.1.250")
#define DEVICE_GW           ("192.168.1.1")
#define DEVICE_SN           ("255.255.255.0")
#define BIT_START           (1 << 0)
#define BIT_STOP            (1 << 1)
#define BIT_CONNECTED       (1 << 2)
#define BIT_DISCONNECTED    (1 << 3)
#define FIVE_MIN            (5*60*1000)

class TcpServer : public TcpSocket, public WifiStation
{
public:

    // Constructor
    TcpServer(port_t Port=TCP_DEFAULT_PORT);

    // Start wifi
    void StartWifi();

    // Start TCP server, start listening
    void StartTcpServer();

    // Accept connections, parse packets
    void HandleConnections();
};