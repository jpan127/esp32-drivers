#include "utilities.h"
#include "string.h"

// class TaskBase
// {
// public:

//     xTaskHandle Handle;

//     ~TaskBase() { vTaskDelete(Handle); }
// };

// // To create a free-function task
// class Task : public TaskBase
// {
// public:

//     Task(char const *name, void (*task_ptr)(void *), uint priority,
//                             u16 stack_depth=configMINIMAL_STACK_SIZE)
//     {
//         xTaskCreate(task_ptr, (const char*)name, stack_depth, this, priority, &Handle);
//     }
// };

// // 
// class TaskClass : public TaskBase
// {
// public:

//     TaskClass(char const *name, uint priority, u16 stack_depth=configMINIMAL_STACK_SIZE)
//     {
//         xTaskCreate(&Task_ptr, (const char*)name, stack_depth, this, priority, &Handle);
//     }

//     virtual void Task() = 0;

//     static void Task_ptr(void *p)
//     {
//         static_cast(p)->Task();
//     }
// };