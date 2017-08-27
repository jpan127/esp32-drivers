#pragma once
#include "pwm.hpp"
#include "gpio.hpp"

// Controls a single L298N, 2 enables, and 2 PWM pins
class MotorDriver
{
public:

    // Constructor
    MotorDriver(gpio_num_t enableA, gpio_num_t enableB, gpio_num_t pwmA, gpio_num_t pwmB);

    MotorDriver(int enableA, int enableB, int pwmA, int pwmB);

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