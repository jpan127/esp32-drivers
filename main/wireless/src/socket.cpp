#include "socket.hpp"

// Functions to convert from network byte order to host byte order or vice versa
// htonl(): host to network long
// htons(): host to network short
// ntohl(): network to host long
// ntohs(): network to host short

Socket::Socket(port_t port)
{
    Port = port;
    Sock = -1;
    State = UNCONNECTED;
}

Socket::~Socket()
{
    Close();
}

void Socket::Close()
{
    close(Sock);
    State = UNCONNECTED;    
}

void Socket::SetPort(port_t port)
{
    Port = port;
}

void Socket::CreateSocket(bool udp)
{
    //                    domain,   type,       protocol
    Sock = (udp) ? (socket(AF_INET, SOCK_DGRAM,  IPPROTO_UDP)) :
                   (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    
    if (Sock == -1) {
        ESP_LOGE("Socket::CreateSocket", "Error creating socket: %s", strerror(errno));
    }
    else {
        ESP_LOGI("Socket::CreateSocket", "Socket successfully created.");
        State = CREATED;
    }
}

void Socket::Bind(bool client, const char *ip)
{
    if (Port == 0) {
        ESP_LOGE("Socket::Bind", "Cannot bind when no port is specified.");
    }
    else if (State == UNCONNECTED) {
        ESP_LOGE("Socket::Bind", "Cannot bind when socket not connected.");
    }
    else if (State == CREATED) {
        struct sockaddr_in server_address;
        server_address.sin_family       = AF_INET;
        server_address.sin_addr.s_addr  = (client) ? inet_addr(ip) : htonl(INADDR_ANY);
        server_address.sin_port         = htons(Port);

        // rc = return code
        int rc = bind(Sock, (struct sockaddr *)&server_address, sizeof(server_address));
        if (rc < 0) {
            ESP_LOGE("Socket::Bind", "Error binding. rc: %i error: %s", rc, strerror(errno));
            Close();
        }
        else {
            ESP_LOGI("Socket::Bind", "Socket successfully binded.");
            State = BINDED;            
        }
    }
}