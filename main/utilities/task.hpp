#pragma once
#include "utilities.h"
#include <string>

// Base class, C++ wrapper for creating tasks
// Should not be used by itself, should be inherited
class Task
{
public:

    Task(std::string task_name, uint16_t stack_size=2048, uint8_t priority=5);

    virtual ~Task();

    void Start(void *p);

    void Stop();

    virtual void Run(void *p) = 0;

    void Delay(int ms);

private:

    static void RunTask(void *task_instance);

    xTaskHandle     Handle;
    std::string     TaskName;
    uint16_t        StackSize;
    uint8_t         Priority;
    void            *Data;
};