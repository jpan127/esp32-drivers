#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>            // Logging
#include <driver/mcpwm.h>       // Pwm

#define DISABLED_PWM (255)

// Select operator A, B, or both
typedef enum {PWM_A, PWM_B, PWM_AB } pwm_output_t;

// Each MCPWM unit has 3 timers
// Each timer has 2 operators : A and B
class Pwm
{
public:

    // Constructor for 2 PWMs
    Pwm(mcpwm_unit_t pwm_unit, int gpio_a, int gpio_b, mcpwm_timer_t timer=MCPWM_TIMER_0);
    // Constructor for only 1 PWM
    Pwm(mcpwm_unit_t pwm_unit, int gpio, mcpwm_timer_t timer=MCPWM_TIMER_0);
    
    // Initializes pwm, the gpio pins, etc with DEFAULT configurations
    // Use other set functions to change the configurations before initialization
    void Initialize();

    // Set PWM Timer, best to call before Initialize()
    void SetTimer(mcpwm_timer_t timer);

    // Set timer frequency, best to call before Initialize()
    void SetFrequency(uint32_t frequency);

    // Sets duty cycle of Timer in percentage
    void SetDuty(float duty_a);
    void SetDuty(float duty_a, float duty_b);

    // Start PWM
    void Start();

    // Stop PWM signals
    void Stop(pwm_output_t pwm_output);

    // Generate PWM on A, B, or both pins
    void GeneratePwm(pwm_output_t pwm_output, float duty);

private:

    // Calculates duty cycle from position
    float PositionToDutyCycle(int8_t position);

    mcpwm_unit_t        PwmUnit;    // PWM Unit, 0 or 1
    int                 GpioA;      // Gpio Pin A
    int                 GpioB;      // Gpio Pin B
    mcpwm_timer_t       PwmTimer;   // PWM Timer, 0 or 1
    mcpwm_config_t      PwmConfig;  // PWM Configuration
};