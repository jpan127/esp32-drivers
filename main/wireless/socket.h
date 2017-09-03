// #include <lwip/sockets.h>

// void tcp_server()
// {
//     // Create a socket: socket(domain, type, protocol)
//     int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// }

// void tcp_client()
// {
//     // Create a socket: socket(domain, type, protocol)
//     int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// }

// void tcp_server_accept_client(int sock)
// {
//     struct sockaddr_in client_addr;
//     socklen_t client_addr_length = sizeof(client_addr);
//     int client_sock = accept(sock, (struct sockaddr *)&client_addr, client_addr_length);
// }

// void tcp_client_connect_server(int sock, char *ip="192.168.1.200")
// {
//     struct sockaddr_in server_addr;
//     server_addr.sin_family = AF_INET;
//     inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr);
//     server_addr.sin_port = htons(9999);
    
//     int rc = connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
// }

// int tcp_last_socket_errno(int socket)
// {
//     int errno = 0;
//     uint32_t optlen = sizeof(errno);

//     getsockopt(socket, SOL_SOCKET, SO_ERROR, &errno, &optlen);

//     // Log socket error?
//     return errno;
// }