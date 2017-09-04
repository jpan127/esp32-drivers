#include "UdpSocket.hpp"

// Maximum number of concurrent SocketReceiveTasks
#define MAX_TASKS (5)
static int num_tasks = 0;

// Acknolwedgement packet
typedef struct {
    const char    ack[4] = "ACK";
    const size_t  size   = 4;
} ack_packet_t;

///////////////////////////////////////////////////////////////////////////////////////////////////

// Send acknowledgement, not in class function so the task can also access
static void SendAck(int sock, struct sockaddr_in client_address)
{
    // Only use this instance of ack_packet_t
    static const ack_packet_t ack_packet;

    socklen_t client_address_size = sizeof(client_address);
    int length = sendto(sock, 
                                ack_packet.ack, 
                                ack_packet.size, 
                                0, 
                                (struct sockaddr *)&client_address,
                                client_address_size);
    if (length < 0) {
        ESP_LOGE("SendAck", "Error sending acknowledgement. Error: %s", strerror(errno));
    }
    else if (length < ack_packet.size) {
        ESP_LOGE("SendAck", "Size of packet sent differs from size specified. %i != %i", 
                                                                        length,
                                                                        ack_packet.size);
    }
}

// [TASK] Parameter is socket handle
static void SocketReceiveTask(void *p_sock)
{
    static const char *TAG = "SocketReceiveTask";
    num_tasks++;

    // Get parameter
    int sock = *(int*)p_sock;

    int buffsize = MAX_BUF;
    uint8_t buffer[buffsize];
    struct sockaddr_in client_address;
    socklen_t sockaddr_size = sizeof(client_address);
    memset(&buffer, 0, buffsize);
    memset(&client_address, 0, sizeof(client_address));
    
    // Cast parameter back to int, then dereference
    int length = recvfrom(sock, 
                                        buffer, 
                                        sizeof(buffer), 
                                        0, 
                                        (struct sockaddr *)&client_address, 
                                        &sockaddr_size);                                        
    
    if (length > 0)
    {
        // Messsage should not be greater than buffsize
        if (length > buffsize) {
            ESP_LOGE(TAG, "Max buffer size exceeded: %i", buffsize);
            buffer[MAX_BUF-1] = '\0';
        }
        // Normal receive
        else {
            buffer[length] = '\0';
        }
        ESP_LOGI(TAG, "Port: %i | Address: %i | Received Packet: %s", 
                                                ntohs(client_address.sin_port),
                                                ntohs(client_address.sin_addr.s_addr),
                                                buffer);
        // Send acknowledgement
        SendAck(sock, client_address);
    }
    // Receive error
    else 
    {
        ESP_LOGE(TAG, "Error receiving. length: %i, error: %s", length, strerror(errno));
    }

    num_tasks--;
    vTaskDelete(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UdpSocket::UdpSocket(port_t Port) : Socket(Port)
{
    /* EMPTY */
}

UdpSocket::~UdpSocket()
{
    Close();
}

void UdpSocket::UdpCreateSocket()
{
    // Create socket udp=true
    CreateSocket(true);
}

void UdpSocket::UdpReceive(bool block)
{
    if (State == BINDED) {
        // If block=false and there are free tasks
        if (!block && num_tasks < MAX_TASKS) {
            ESP_LOGI("UdpSocket::UdpReceive", "Creating SocketReceiveTask %i...", num_tasks);
            xTaskCreate(&SocketReceiveTask, "SocketReceiveTask", 4096, (void *)&Sock, 5, NULL);
        }
        // Otherwise block
        else {
            UdpReceiveFunction();
        }
    }
    else {
        ESP_LOGE("Socket::Receive", "Cannot receive if socket is not binded.");
    }
}

void UdpSocket::UdpReceiveFunction()
{
    static const char *TAG = "UdpSocket::UdpReceiveFunction";

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

    if (length > 0)
    {
        // Messsage should not be greater than buffsize
        if (length > buffsize) {
            ESP_LOGE(TAG, "Max buffer size exceeded: %i", buffsize);
            buffer[MAX_BUF-1] = '\0';
        }
        // Normal receive
        else {
            buffer[length] = '\0';
        }
        ESP_LOGI(TAG, "Port: %i | Address: %i | Received Packet: %s", 
                                                ntohs(client_address.sin_port),
                                                ntohs(client_address.sin_addr.s_addr),
                                                buffer);
        // Send acknowledgement
        SendAck(Sock, client_address);
    }
    // Receive error
    else 
    {
        ESP_LOGE(TAG, "Error receiving. length: %i, error: %s", length, strerror(errno));
    }
}