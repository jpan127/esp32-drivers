#pragma once
#include "pwm.hpp"
#include "gpio.hpp"

// Controls a single L298N, 2 enables, and 2 PWM pins
class MotorDriver
{
public:

    // Constructor
    MotorDriver(int enableA, int enableB, 
                int pwmA,    int pwmB,
                mcpwm_unit_t pwm_unit, mcpwm_timer_t timer);

    // Move forward
    void Forward(float duty);

    // Move backward
    void Backward(float duty);

    // Stop
    void Stop();

    // Turn left
    void Left(float duty);
    
    // Turn right
    void Right(float duty);

private:

    // Enable pins for L298N, HIGH for forward, LOW for backward, or vice versa
    GpioOutput  EnableA;
    GpioOutput  EnableB;
    // Pwm pins for 2 motors
    Pwm         Motors;
};