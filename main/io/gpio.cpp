#include "gpio.hpp"


Gpio::Gpio(gpio_num_t pin, gpio_mode_t mode)
{
    Pin = pin;

    // Default configuration
    gpio_config_t config;
    config.intr_type    = GPIO_INTR_DISABLE;
    config.mode         = mode;
    config.pin_bit_mask = 1 << Pin;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&config));

    // If output, clear to known state
    if (mode == GPIO_MODE_OUTPUT) {
        Clear();
    }
}

Gpio::Gpio(int pin, gpio_mode_t mode)
{
    // Converting int to gpio_num_t
    // For some reason 20, 24, 28-30 aren't included
    gpio_num_t _pin;
    switch (pin)
    {
        case 0: _pin = GPIO_NUM_0; break;
        case 1: _pin = GPIO_NUM_1; break;
        case 2: _pin = GPIO_NUM_2; break;
        case 3: _pin = GPIO_NUM_3; break;
        case 4: _pin = GPIO_NUM_4; break;
        case 5: _pin = GPIO_NUM_5; break;
        case 6: _pin = GPIO_NUM_6; break;
        case 7: _pin = GPIO_NUM_7; break;
        case 8: _pin = GPIO_NUM_8; break;
        case 9: _pin = GPIO_NUM_9; break;
        case 10: _pin = GPIO_NUM_10; break;
        case 11: _pin = GPIO_NUM_11; break;
        case 12: _pin = GPIO_NUM_12; break;
        case 13: _pin = GPIO_NUM_13; break;
        case 14: _pin = GPIO_NUM_14; break;
        case 15: _pin = GPIO_NUM_15; break;
        case 16: _pin = GPIO_NUM_16; break;
        case 17: _pin = GPIO_NUM_17; break;
        case 18: _pin = GPIO_NUM_18; break;
        case 19: _pin = GPIO_NUM_19; break;
        // case 20: _pin = GPIO_NUM_20; break;
        case 21: _pin = GPIO_NUM_21; break;
        case 22: _pin = GPIO_NUM_22; break;
        case 23: _pin = GPIO_NUM_23; break;
        // case 24: _pin = GPIO_NUM_24; break;
        case 25: _pin = GPIO_NUM_25; break;
        case 26: _pin = GPIO_NUM_26; break;
        case 27: _pin = GPIO_NUM_27; break;
        // case 28: _pin = GPIO_NUM_28; break;
        // case 29: _pin = GPIO_NUM_29; break;
        // case 30: _pin = GPIO_NUM_30; break;

        default:
            ESP_LOGE("Gpio::Gpio", "Nonexistent GPIO pin, GPIO uninitialized.");
            return;
    }
    Pin = _pin;

    // Default configuration
    gpio_config_t config;
    config.intr_type    = GPIO_INTR_DISABLE;
    config.mode         = mode;
    config.pin_bit_mask = 1 << Pin;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&config));

    // If output, clear to known state
    if (mode == GPIO_MODE_OUTPUT) {
        Clear();
    }
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

void Gpio::Set()
{
    ESP_ERROR_CHECK(gpio_set_level(Pin, 1));
}

void Gpio::Clear()
{
    ESP_ERROR_CHECK(gpio_set_level(Pin, 0));
}

void Gpio::SetResistorMode(gpio_pull_mode_t pull)
{
    ESP_ERROR_CHECK(gpio_set_pull_mode(Pin, pull));
}