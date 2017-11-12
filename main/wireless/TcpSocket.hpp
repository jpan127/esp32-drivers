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

    // Copy Constructor
    TcpSocket(const TcpSocket &copy);    

    // Assignment Constructor
    TcpSocket& operator=(TcpSocket copy);

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
    friend void AcceptConnection(TcpSocket tcp_socket, const char* TAG, int num_tasks);

    // [BLOCKING] Parse data packet of accepted connection
    void Receive(int client_sock);
};