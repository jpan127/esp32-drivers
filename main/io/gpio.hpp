#pragma once
#include "utilities.h"
#include <driver/gpio.h>

typedef uint8_t pin_t;

// Class to wrap GPIO initilization
class Gpio
{
protected:

    // Constructor
    Gpio(gpio_num_t pin, gpio_mode_t mode);
    Gpio(int pin, gpio_mode_t mode);

    // Sets interrupt type
    void SetInterrupt(gpio_int_type_t intr_type);

    // Enables interrupt
    void EnableInterrupt();

    // Disables interrupt
    void DisableInterrupt();
    
    // Sets pull up/down/none mode of resistor
    void SetResistorMode(gpio_pull_mode_t pull);

    // Pin number
    gpio_num_t Pin;
};

// Child class of Gpio that starts out as Input
class GpioInput : public Gpio
{
public:

    GpioInput(gpio_num_t pin) : Gpio(pin, GPIO_MODE_INPUT) { /* EMPTY */ }

    GpioInput(int pin) : Gpio(pin, GPIO_MODE_INPUT) { /* EMPTY */ }
};

// Child class of Gpio that starts out as Output
class GpioOutput : public Gpio
{
public:

    GpioOutput(gpio_num_t pin) : Gpio(pin, GPIO_MODE_OUTPUT) { Clear(); }

    GpioOutput(int pin) : Gpio(pin, GPIO_MODE_OUTPUT) { Clear(); }

    // Sets HIGH
    void Set();
    
    // Sets LOW
    void Clear();
};