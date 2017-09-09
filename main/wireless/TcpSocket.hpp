#pragma once
#include "socket.hpp"
#include <string>

//////////////////////////////////////////////////
//  How to use TcpSocket                        //
//                                              //
//      Server:                                 //
//          1. Create socket                    //
//          2. Listen for connections           //
//          3. Accept a connection request      //
//          4. Receive the data, close socket   //
//      Client:                                 //
//                                              //
//          1. Create socket                    //
//          2. Connect to server                //
//          3. Send packet                      //
//                                              //
//////////////////////////////////////////////////

class TcpSocket : public Socket
{
public:

    // Constructor
    TcpSocket(port_t Port);

    // Destructor
    ~TcpSocket();

    // Create TCP socket
    void CreateTcpSocket();

    // [BLOCKING] Connect to server
    void ConnectToServer(char *server_ip, int server_port);

    // [BLOCKING] Send packet
    void Send(char *packet);
    void Send(std::string packet);

    // Listening for connections
    void Listen(uint8_t queue_size);

    // [BLOCKING] Accept a connection on a socket
    void Accept();

    // Returns socket handle
    int GetSock();

    // So this function can access Receive()
    friend void AcceptConnection(TcpSocket tcp_socket, const char* TAG);

private:

    // [BLOCKING] Parse data packet of accepted connection
    void Receive(int client_sock);
};

// void tcp_client_connect_server(int sock, char *ip="192.168.1.200")
// {
//     struct sockaddr_in server_addr;
//     server_addr.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr);
//     server_addr.sin_port = htons(9999);
    
//     int rc = connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
// }