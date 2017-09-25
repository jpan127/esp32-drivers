#include "gpio.hpp"

Gpio::Gpio(gpio_num_t pin, gpio_mode_t mode)
{
    Pin = pin;

    // Default configuration
    gpio_config_t config;
    config.intr_type    = GPIO_INTR_DISABLE;
    config.mode         = mode;
    config.pin_bit_mask = 1 << gpio_enum_to_int(Pin);
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&config));
}

Gpio::Gpio(int pin, gpio_mode_t mode)
{
    // Converting int to gpio_num_t
    Pin = gpio_int_to_enum(pin);

    // Default configuration
    gpio_config_t config;
    config.intr_type    = GPIO_INTR_DISABLE;
    config.mode         = mode;
    config.pin_bit_mask = 1 << pin;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&config));
}

void Gpio::SetInterrupt(gpio_int_type_t intr_type)
{
    ESP_ERROR_CHECK(gpio_set_intr_type(Pin, intr_type));
}

void Gpio::EnableInterrupt()
{
    ESP_ERROR_CHECK(gpio_intr_enable(Pin));
}

void Gpio::DisableInterrupt()
{
    ESP_ERROR_CHECK(gpio_intr_disable(Pin));    
}

void Gpio::SetResistorMode(gpio_pull_mode_t pull)
{
    ESP_ERROR_CHECK(gpio_set_pull_mode(Pin, pull));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void GpioOutput::Set()
{
    ESP_ERROR_CHECK(gpio_set_level(Pin, 1));
}

void GpioOutput::Clear()
{
    ESP_ERROR_CHECK(gpio_set_level(Pin, 0));
}