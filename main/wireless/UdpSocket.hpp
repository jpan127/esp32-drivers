#pragma once
#include "socket.hpp"

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

private:

    void UdpReceiveFunction();
};