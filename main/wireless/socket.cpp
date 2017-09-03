#include "socket.hpp"

// Parameter is socket handle
static void SocketReceiveTask(void *p_sock)
{
    int buffsize = MAX_BUF;
    uint8_t buffer[buffsize];
    struct sockaddr_in client_address;
    socklen_t sockaddr_size = sizeof(client_address);
    memset(&buffer, 0, buffsize);
    memset(&client_address, 0, sizeof(client_address));
    
    // Cast parameter back to int, then dereference
    int length = recvfrom(*(int*)p_sock, 
                                        buffer, 
                                        sizeof(buffer), 
                                        0, 
                                        (struct sockaddr *)&client_address, 
                                        &sockaddr_size);                                        
                                                                              
    // Messsage should not be greater than buffsize
    if (length > buffsize) {
        ESP_LOGE("SocketReceiveTask", "Max buffer size exceeded: %i", buffsize);
        buffer[255] = '\0';
        ESP_LOGI("SocketReceiveTask", "Receive Packet (Truncated): %s", buffer);

        // // Send acknowledgement
        // const char *ack = "ACK";
        // size_t size = 4;
        // int rc = sendto(*(int*)p_sock), 
    }
    // Normal receive
    else if (length > 0) {
        buffer[length] = '\0';
        ESP_LOGI("SocketReceiveTask", "Receive Packet: %s", buffer);
    }
    // Receive error
    else {
        ESP_LOGE("SocketReceiveTask", "Error receiving. length: %i, error: %s", length, strerror(errno));
    }

    vTaskDelete(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

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
        State = CONNECTED;
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
    else if (State == CONNECTED) {
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

void Socket::Receive(bool block)
{
    if (State == BINDED) {
        if (block) {
            SocketReceiveFunction();
        }
        else {
            xTaskCreate(&SocketReceiveTask, "SocketReceiveTask", 2048, (void *)&Sock, 5, NULL);
        }      
    }
    else {
        ESP_LOGE("Socket::Receive", "Cannot receive if socket is not binded.");
    }
}

void Socket::SocketReceiveFunction()
{
    int buffsize = MAX_BUF;
    uint8_t buffer[buffsize];
    struct sockaddr_in client_address;
    socklen_t sockaddr_size = sizeof(client_address);
    memset(&buffer, 0, buffsize);
    memset(&client_address, 0, sizeof(client_address));

    int length = recvfrom(Sock, 
                                buffer, 
                                sizeof(buffer), //buffsize 
                                0, 
                                (struct sockaddr *)&client_address, 
                                &sockaddr_size);

    // Messsage should not be greater than buffsize
    if (length > buffsize) {
        ESP_LOGE("SocketReceiveTask", "Max buffer size exceeded: %i", buffsize);
        buffer[MAX_BUF-1] = '\0';
        ESP_LOGI("SocketReceiveTask", "Received Packet (Truncated): %s", buffer);
    }
    // Normal receive
    else if (length > 0) {
        buffer[length] = '\0';
        ESP_LOGI("SocketReceiveTask", "Received Packet: %s", buffer);
    }
    // Receive error
    else {
        ESP_LOGE("SocketReceiveTask", "Error receiving. length: %i, error: %s", length, strerror(errno));
    }
}


// void Socket::Listen()
// {
//     // TCP only
//     // Size of socket connections queue
//     int listen_queue = 5;

//     int rc = listen(Sock, listen_queue);
//     if (rc < 0) {
//         ESP_LOGE("Socket::Listen", "Error listening. rc: %i error: %s", rc, strerror(errno));
//     }
//     else {
//         State = LISTENING;
//     }
// }

// void Socket::Accept()
// {
//     // TCP only
// }
