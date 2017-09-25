#pragma once
#include "pwm.hpp"
#include "gpio.hpp"

#define PERIOD      (20)    // Total period of PWM cycle in milliseconds
#define PULSE_MIN   (1)     // Min pulse time in milliseconds
#define PULSE_MID   (1.5)   // Medium pulse time in milliseconds
#define PULSE_MAX   (2)     // Max pulse time in milliseconds

typedef enum { SWEEP_SLOW, SWEEP_NORMAL, SWEEP_FAST } servo_sweep_speed_t;

// Calculate frequency : f = 1/T in which T is in milliseconds
// 50Hz for 20ms period
const float FREQUENCY = 1 / (PERIOD * 0.001);

// Typical Pins
// Brown    : Ground
// Red      : VCC
// Orange   : Control

class Servo : public Pwm
{
public:

    // Constructor
    Servo(mcpwm_unit_t pwm_unit, pin_t pwm_pin);

    // Converts position to duty cycle
    float PositionToDutyCycle(int8_t position);

    // Set to any position from -90 to +90
    void SetPosition(int8_t position);

    // Set to +90
    void SetMaxPositive();

    // Set to 0
    void SetStraight();

    // Set to -90
    void SetMaxNegative();

    // Sweep continuously from -90 to +90 and back
    void Sweep(servo_sweep_speed_t speed);

private:

};