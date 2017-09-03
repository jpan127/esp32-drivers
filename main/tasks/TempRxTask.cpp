#include "UdpAccessPoint.cpp"
#include "utilities.h"

void TempRxTask(void *p)
{
    // Initialize flash
    ESP_ERROR_CHECK(nvs_flash_init());

    // Setup as UDP server
    UdpAccessPoint udp_server;

    udp_server.Initialize();
    udp_server.InitializeSocket();

    while (1)
    {
        udp_server.WaitForPacket();
        
        // When unblocked
        TASK_DELAY(1000);
    }
}

// void PacketTestTask(void *p)
// {
//     // Initialize flash
//     ESP_ERROR_CHECK(nvs_flash_init());

//     int sock = socket(AF_INET, SOCK_DGRAM,  IPPROTO_UDP);
//     if (sock == -1) {
//         ESP_LOGE("Socket::CreateSocket", "Error creating socket: %s", strerror(errno));
//     }
//     else {
//         ESP_LOGI("Socket::CreateSocket", "Socket successfully created.");
//         State = CONNECTED;
//     }

//     struct sockaddr_in server_address;
//     server_address.sin_family       = AF_INET;
//     server_address.sin_addr.s_addr  = htonl(INADDR_ANY);
//     server_address.sin_port         = htons(Port);

//     // rc = return code
//     int rc = bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
//     if (rc < 0) {
//         ESP_LOGE("Socket::Bind", "Error binding. rc: %i error: %s", rc, strerror(errno));
//         Close();
//     }
//     else {
//         ESP_LOGI("Socket::Bind", "Socket successfully binded.");
//         State = BINDED;            
//     }

//     int buffsize = MAX_BUF;
//     uint8_t buffer[buffsize];
//     struct sockaddr_in client_address;
//     memset(&client_address, 0, sizeof(client_address));

//     // Cast parameter back to int, then dereference
//     int length = recvfrom(sock, 
//                                 buffer, 
//                                 sizeof(buffer), 
//                                 0, 
//                                 (struct sockaddr *)&client_address, 
//                                 (socklen_t *)(sizeof(client_address)));

//     if (length > 0) {
//         buffer[length] = '\0';
//         ESP_LOGI("SocketReceiveTask", "Receive Packet: %s", buffer);
//     }
// }