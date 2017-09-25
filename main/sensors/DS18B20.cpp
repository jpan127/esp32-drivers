#include "DS18B20.hpp"

DS18B20::DS18B20(uint8_t pin)
{
    ESP_LOGI("DS18B20", "Initializing sensor GPIO pin.");
    Pin = pin;
    gpio_pad_select_gpio(Pin);
}

void DS18B20::SetOutput()
{
    gpio_set_direction(gpio_int_to_enum(Pin), GPIO_MODE_OUTPUT);    
}

void DS18B20::SetInput()
{
    gpio_set_direction(gpio_int_to_enum(Pin), GPIO_MODE_INPUT);    
}

void DS18B20::SetLow()
{
    gpio_set_level(gpio_int_to_enum(Pin), 0);
}

void DS18B20::SetHigh()
{
    gpio_set_level(gpio_int_to_enum(Pin), 1);
}

void DS18B20::SendBit(bool bit)
{
    // All write time slots must be a minimum of 60 us in duration
    // and 1 us recovery time between writes
    SetOutput();
    // Master pulls low to initiate write
    SetLow();
    // Generate write 1 time slot
    if (bit)
    {
        // Must release within 15 us
        DELAY(10);
        // Master release bus
        SetHigh();
        // Remainder of sampling period
        DELAY(60);
    }
    // Generate write 0 time slot
    else
    {
        // Hold for at least 60 us
        DELAY(70);
        // Master release bus
        SetHigh();        
    }
}

void DS18B20::SendByte(uint8_t byte)
{
    // LSB first
    for (int i=0; i<8; i++) {
        SendBit( (byte >> i) & 1 );
        DELAY(2);
    }
    DELAY(100);
}

bool DS18B20::ReceiveBit()
{
    // Master pulls bus low for a minimum of 1 us
    SetOutput();
    SetLow();
    DELAY(2);
    // Master releases bus to initiate read time slot
    SetHigh();
    // Read bus from slave
    SetInput();
    // Wait a little before sampling
    DELAY(10);
    return gpio_get_level(gpio_int_to_enum(Pin));
}

uint8_t DS18B20::ReceiveByte()
{
    uint8_t byte = 0;
    for (int i=0; i<8; i++) {
        byte |= (ReceiveBit() << i);
        DELAY(15);
    }
    return byte;
}

bool DS18B20::ResetPulse()
{
    // Reset pulse: pull bus low for minimum 480 us
    SetOutput();
    SetLow();
    DELAY(500);
    // Master releases bus and slave transmits presence pulse
    SetHigh();
    SetInput();
    DELAY(500);
    return gpio_get_level(gpio_int_to_enum(Pin));
}

float DS18B20::ReadTemperature()
{
    ESP_LOGI("DS18B20::ReadTemperature", "Reading temperature...");
    uint8_t t1, t2;
    float   t3;
    if (ResetPulse())
    {
        // Initialize and convert temperature
        SendByte(SKIP_ROM_OPCODE);
        SendByte(CONVERT_T_OPCODE);
        // Reset
        DELAY(750000);
        ResetPulse();
        // Initialize and read from scratchpad registers
        SendByte(SKIP_ROM_OPCODE);
        SendByte(READ_SCRATCH_OPCODE);
        // Read first 2 bytes and combine into one value
        t1 = ReceiveByte();
        t2 = ReceiveByte();
        t3 = (float)(t1 + t2*256) / 16;
        ESP_LOGI("DS18B20::ReadTemperature", "t1: %i | t2: %i | t3: %f", t1, t2, t3);
        ResetPulse();        
        return CelsiusToFarenheit(t3);
    }
    else
    {
        // Not sure if necessary
        return 0;
    }
}

float DS18B20::CelsiusToFarenheit(float c)
{
    return (c * 1.8) + 32;
}