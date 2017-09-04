#pragma once
#include "utilities.h"
#include <driver/gpio.h>

// Class to wrap GPIO initilization
class Gpio
{
public:

    // Constructor
    Gpio(gpio_num_t pin, gpio_mode_t mode);
    
    Gpio(int pin, gpio_mode_t mode);

    // Sets HIGH
    virtual void Set();
    
    // Sets LOW
    virtual void Clear();

    // Sets interrupt type
    virtual void SetInterrupt(gpio_int_type_t intr_type);

    // Enables interrupt
    virtual void EnableInterrupt();

    // Disables interrupt
    virtual void DisableInterrupt();

    // Sets pull up/down/none mode of resistor
    virtual void SetResistorMode(gpio_pull_mode_t pull);

private:

    gpio_num_t Pin;
};

// Child class of Gpio that starts out as Input
class GpioInput : public Gpio
{
public:

    GpioInput(gpio_num_t pin) : Gpio(pin, GPIO_MODE_INPUT) {}

    GpioInput(int pin) : Gpio(pin, GPIO_MODE_INPUT) {}
};

// Child class of Gpio that starts out as Output
class GpioOutput : public Gpio
{
public:

    GpioOutput(gpio_num_t pin) : Gpio(pin, GPIO_MODE_OUTPUT) {}

    GpioOutput(int pin) : Gpio(pin, GPIO_MODE_OUTPUT) {}    
};