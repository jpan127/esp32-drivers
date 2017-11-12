#pragma once
#include "TcpServer.hpp"
#include "Task.hpp"

#define TCP_PORT (4000)

class TcpServerTask : public Task, public TcpServer
{
public:

    TcpServerTask(  std::string task_name="TcpServerTask", 
                    uint16_t stack_size=2048,
                    uint8_t priority=5) : 
                    Task(task_name, stack_size, priority),
                    TcpServer(TCP_PORT)
    {
        /* EMPTY */
    }

    void Run(void *p)
    {
        // Get wifi running
        StartWifi();
        // Start listening to TCP port
        StartTcpServer();

        while (1)
        {
            // Creates 5 blocking tasks, then blocks this main task
            HandleConnections();
        }        
    }
};