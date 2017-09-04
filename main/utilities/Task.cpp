#include "Task.hpp"

Task::Task(std::string task_name, uint16_t stack_size, uint8_t priority)
{
    TaskName    = task_name;
    StackSize   = stack_size;
    Handle      = nullptr;
    Data        = nullptr; 
}

Task::~Task()
{
    /* EMPTY */
}

void Task::Start(void *p)
{
    Data = p;

    xTaskCreate(&RunTask, TaskName.c_str(), StackSize, this, Priority, &Handle);
}

void Task::Stop()
{
    // Not created yet
    if (Handle == nullptr) return;

    // Set task handle to null before deletion
    xTaskHandle temp = Handle;
    Handle = nullptr;
    vTaskDelete(temp);
}

void Task::Delay(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void Task::RunTask(void *task_instance)
{
    // Create instance and run
    Task *task = (Task *)task_instance;
    task->Run(task->Data);

    // Should only reach here if Run exits
    task->Stop();
    
    static const char *TAG = task->TaskName.c_str();
    ESP_LOGI(TAG, "Exited task.");
}