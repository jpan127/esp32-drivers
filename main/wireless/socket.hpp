#pragma once
#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
#include <lwip/inet.h>                  // inet functions, socket structs
#include <lwip/sockets.h>               // Sockets
#include <string.h>                     // String
#include <errno.h>                      // Determining errors
#include <cstring>                      // memset() memcpy()

#define MAX_BUF (1024)

typedef uint16_t port_t;
typedef enum {UNCONNECTED, CREATED, BINDED, CONNECTED, LISTENING} socket_state_t;

//////////////////////////////////////////////////////
//  How to use Socket:                              //
//                                                  //
//      Server:                                     //
//          1. Create socket                        //
//          2. Bind socket to port                  //
//          3. TCP = listen, UDP = recvfrom         //
//          4. TCP = accept                         //
//                                                  //
//      Client:                                     //
//          1. Create socket                        //
//          2. TCP = connect to server              //
//          3. TCP = send data                      //
//                                                  //
//////////////////////////////////////////////////////

class Socket
{
public:

    // Deletes socket
    ~Socket();

    // Closes socket, called by destructor
    void Close();
        
    // Set a different port
    void SetPort(port_t port);

    // Creates a socket, TCP or UDP
    void CreateSocket(bool udp);

    // Binds socket to port
    // Set client to true to bind as a client, must also set ip address
    // Otherwise default to server, no ip address needed
    void Bind(bool client=false, const char *ip="0.0.0.0");

    port_t GetPort();

protected:

    // Constructor
    Socket(port_t port);
        
    port_t          Port;
    int             Sock;
    socket_state_t  State;
};