#pragma once
#include "UdpSocket.hpp"
#include "WifiStation.hpp"
#include "utilities.h"
#include "credentials.hpp"          // Saved ssid and password
#include <freertos/event_groups.h>

class UdpClient : public WifiStation
{
public:

    UdpClient(port_t Port);

    // Initialize wifi
    void Initialize();
        
    // Initializes socket
    void InitializeSocket();

    // Send packet
    void SendPacket(char *packet, int packet_length, port_t port, char *server_ip);

private:

    UdpSocket mClientSocket;
};