#pragma once
#include "UdpServer.cpp"
#include "utilities.h"
#include "Task.hpp"

class UdpServerTask : public Task
{
public:

    UdpServerTask(std::string task_name="UdpServerTask", 
                                            uint16_t stack_size=2048,
                                            uint8_t priority=5) : 
                                            Task(task_name, stack_size, priority)
    {
        /* EMPTY */
    }

    void Run(void *p)
    {
        // Setup as UDP server
        UdpServer udp_server;
        // Initialize wifi as station
        udp_server.Initialize();
        // Initialize socket for receiving
        udp_server.InitializeSocket();

        while (1)
        {
            udp_server.WaitForPacket(false);
            
            // When unblocked
            TASK_DELAY(100);
        }
    }

};