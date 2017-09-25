#pragma once
#include <cstdio>
#include "UdpClient.hpp"
#include "utilities.h"
#include "TemperatureReadTask.hpp"
#include "Task.hpp"

#define PYTHON_SERVER_PORT  (11111)
#define PYTHON_SERVER_IP    ("192.168.1.229")

class UdpClientTask : public Task
{
public:

    UdpClientTask(std::string task_name="UdpClientTask", 
                                            uint16_t stack_size=2048,
                                            uint8_t priority=5) : 
                                            Task(task_name, stack_size, priority)
    {
        /* EMPTY */
    }

    void Run(void *p)
    {
        UdpClient udp_client(UDP_DEFAULT_PORT);
        udp_client.Initialize();
        udp_client.InitializeSocket();

        char *packet = new char[1024];
        
        while (1)
        {
            float temp = read_temperature(3);
            sprintf(packet, "bedroom, %f", temp);
            udp_client.SendPacket(packet, 
                                    strlen(packet), 
                                    PYTHON_SERVER_PORT, 
                                    (char *)PYTHON_SERVER_IP);
            // When unblocked
            TASK_DELAY(5000);
        }
    }

};