#pragma once
#include "socket.hpp"

#define DESKTOP_IP          ("192.168.1.229")
#define UDP_DEFAULT_PORT    (4222)
#define DEVICE_IP           ("192.168.1.250")
#define DEVICE_GW           ("192.168.1.1")
#define DEVICE_SN           ("255.255.255.0")
#define BIT_START           (1 << 0)
#define BIT_STOP            (1 << 1)
#define BIT_CONNECTED       (1 << 2)
#define BIT_DISCONNECTED    (1 << 3)
#define FIVE_MIN            (5*60*1000)

// Inherits Bind(), Close()
class UdpSocket : public Socket
{
public:

    // Constructor
    UdpSocket(port_t Port);

    // Destructor
    ~UdpSocket();

    // Creates UDP socket
    void UdpCreateSocket();

    // Receives, block=true to receive in a function
    // block=false to create a new task to receive
    void UdpReceive(bool block=true);

    // Send a packet to server_ip at port
    void UdpSend(char *packet, int packet_length, port_t port, char *server_ip);

private:

    void UdpReceiveFunction();
};