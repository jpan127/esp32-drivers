#include "motor.hpp"

// GPIO pins
// Can't use pins 6-11
#define ENA      ((int)17)
#define ENB      ((int)16)
#define PWMA     ((int)4)
#define PWMB     ((int)0)
#define PWMUNIT  ((mcpwm_unit_t)MCPWM_UNIT_0)
#define PWMTIMER ((mcpwm_timer_t)MCPWM_TIMER_0)

void MotorDriverTask(void *p)
{    
    static const char *TAG = "MotorDriverTask";

    // Initialize
    MotorDriver motor_driver(ENA, ENB, PWMA, PWMB, PWMUNIT, PWMTIMER);
    ESP_LOGI(TAG, "MotorDriver initialized.");

    while (1)
    {
        ESP_LOGI(TAG, "Forward.");
        motor_driver.Forward((float)100);
        TASK_DELAY(5000);

        ESP_LOGI(TAG, "Backward.");
        motor_driver.Backward((float)50);
        TASK_DELAY(5000);

        ESP_LOGI(TAG, "Stop.");
        motor_driver.Stop();
        TASK_DELAY(5000);
    }

    vTaskDelete(NULL);
}