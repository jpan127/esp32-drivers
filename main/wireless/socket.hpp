#include <esp_log.h>                    // Logging
#include <freertos/FreeRTOS.h>          // FreeRTOS
#include <freertos/task.h>              // Create tasks
#include <lwip/inet.h>
#include <lwip/sockets.h>
#include <string.h>
#include <errno.h>
#include <cstring>                      // memset()

#define MAX_BUF (1024)

typedef uint16_t port_t;
typedef enum {UNCONNECTED, CONNECTED, BINDED, LISTENING} socket_state_t;

// Create a socket, bind to port, then listen or receive
class Socket
{
public:

    // Constructor
    Socket(port_t port);

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

protected:

    port_t          Port;
    int             Sock;
    socket_state_t  State;
};