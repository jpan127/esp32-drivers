#include "TcpSocket.hpp"

// Maximum number of concurrent TcpSocketAcceptTask
#define MAX_TASKS (5)
static int num_tasks = 0;

TcpSocket::TcpSocket(port_t Port) : Socket(Port)
{
    /* EMPTY */
}

TcpSocket::~TcpSocket()
{
    Close();
}

void TcpSocket::CreateTcpSocket()
{
    CreateSocket(false);
}

void TcpSocket::ConnectToServer(char *server_ip, int server_port)
{
    // Create socket address struct for the server's address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr);
    server_address.sin_port = htons(server_port);

    // Connect to the server
    int rc = connect(Sock, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
    if (rc < 0) {
        ESP_LOGE("TcpSocket::ConnectToServer", "Error connecting to server at %s port %i. Error: %s",
                                                                    server_ip, Port, strerror(errno));
    }
    else {
        ESP_LOGI("TcpSocket::ConnectToServer", "Successfully connected to server at %s port %i.",
                                                                    server_ip, Port);
        State = CONNECTED;
    }
}

void TcpSocket::Send(char *packet)
{
    int rc = send(Sock, packet, strlen(packet), 0);
    if (rc < 0) {
        ESP_LOGE("TcpSocket::Send", "Error sending packet. Error: %s", strerror(errno));
    }
    else {
        ESP_LOGI("TcpSocket::Send", "Packet sent: %s", packet);        
    }
}

void TcpSocket::Send(std::string packet)
{
    int rc = send(Sock, packet.c_str(), packet.length(), 0);
    if (rc < 0) {
        ESP_LOGE("TcpSocket::Send", "Error sending packet. Error: %s", strerror(errno));
    }
    else {
        ESP_LOGI("TcpSocket::Send", "Packet sent: %s", packet.c_str());        
    }
}

void TcpSocket::Listen(uint8_t queue_size=MAX_TASKS+1)
{
    // Does not block
    int rc = listen(Sock, queue_size);
    if (rc < 0) {
        ESP_LOGE("TcpSocket::Listen", "Error starting to listen on socket. Error: %s", strerror(errno));
    }
    else {
        ESP_LOGI("TcpSocket::Listen", "Listening on socket port %i", Port);
        State = LISTENING;
    }
}

int TcpSocket::GetSock()
{
    return Sock;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// [TODO] Encapsulate AcceptConnection + TcpSocketAcceptTask inside TcpSocket

// Helper function that accepts a connection, used by task + member function
void AcceptConnection(TcpSocket tcp_socket, const char* TAG)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    // Receive socket handle of accepted connection
    int client_sock = accept(tcp_socket.GetSock(), 
                            (struct sockaddr *)&client_address, 
                            &client_address_size);
    if (client_sock < 0) {
        ESP_LOGE(TAG, "Error accepting from client. Error: %s", strerror(errno));
    }
    else {
        ESP_LOGI(TAG, "Accepted client connection. Socket: %i", client_sock);
        tcp_socket.Receive(client_sock);
    }
}

// [TASK] Task that accepts a connection and blocks
static void TcpSocketAcceptTask(void *TcpSocketInstance)
{
    num_tasks++;
    
    AcceptConnection(*(TcpSocket *)TcpSocketInstance, "TcpSocketAcceptTask");

    num_tasks--;
    vTaskDelete(NULL);
}

void TcpSocket::Accept()
{
    if (num_tasks < MAX_TASKS) {
        ESP_LOGI("TcpSocket::Accept", "Creating TcpSocketAcceptTask %i", num_tasks);
        xTaskCreate(&TcpSocketAcceptTask, "TcpSocketAcceptTask", 2048, (void *)&Sock, 5, NULL);
    }
    // This is so the main task also blocks, remove this section if main task needs to do other things
    else {
        ESP_LOGI("TcpSocket::Accept", "Already running max TcpSocketAcceptTasks!");
        ESP_LOGI("TcpSocket::Accept", "Main task blocking...");
        AcceptConnection(*this, "TcpSocket::Accept");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Helper function for when the data buffer is too small
static void double_buffer_size(char **buffer, size_t *current_size)
{
    if (*buffer == NULL) {
        ESP_LOGE("double_buffer_size", "Pointer argument null.");
        return;
    }

    // Create larger buffer
    char *doubled_buffer = new char[*current_size * 2];
    // Copy contents over
    memcpy(doubled_buffer, *buffer, *current_size);
    // Assign pointer to larger pointer's address
    delete [] *buffer;
    *buffer = doubled_buffer;

    *current_size *= 2;    
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// http://man7.org/linux/man-pages/man2/recv.2.html
void TcpSocket::Receive(int client_sock)
{
    static const char *TAG = "TcpSocket::Receive";

    // Holds the current buffer's size, can change
    size_t current_buffer_size = MAX_BUF;
    char *data = new char[current_buffer_size];
    // Show how filled up the buffer is
    int buffer_index = 0;

    // While there is still data to be read from socket
    while (1)
    {
        // ssize_t not size_t because ssize_t handles negative (error) return
        // Read data from socket
        ssize_t size_read = recv(client_sock, 
                                // Fill buffer from where it was last filled to
                                data + buffer_index, 
                                current_buffer_size - buffer_index, 
                                0);
        if (size_read < 0) {
            ESP_LOGE(TAG, "Error receiving on socket %i. Error: %s", client_sock, strerror(errno));
            return;
        }
        // Nothing more to read
        else if (size_read == 0) {
            break;
        }
        else {
            buffer_index += size_read;
            // Buffer overflowed
            if (buffer_index > current_buffer_size) {
                ESP_LOGE(TAG, "Receive buffer overflowed. Data may be lost.");
                ESP_LOGE(TAG, "Resizing buffer...");
                double_buffer_size(&data, &current_buffer_size);
            }
        }
    }

    ESP_LOGI(TAG, "Received %i bytes : %.*s", buffer_index, buffer_index, data);
    // Free buffer
    delete [] data;
    // Close client socket since we are done with it
    close(client_sock);
}