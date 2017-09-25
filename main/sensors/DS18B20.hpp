#pragma once
#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <esp_log.h>
#include "utilities.h"

#define DELAY(us)           (ets_delay_us(us))
// Search for devices
#define SEARCH_ROM_OPCODE   (0xF0)
// Allows bus master to read slave's 64 bit ROM code without searching for it
#define READ_ROM_OPCODE     (0x33)
// Addresses a single slave by the ROM code
#define MATCH_ROM_OPCODE    (0x55)
// Skip ROM, address all devices
#define SKIP_ROM_OPCODE     (0xCC)
// Initiate temperature conversion
#define CONVERT_T_OPCODE    (0x44)
// Read scratchpad 9 bytes, LSBit and LSByte first, stop by reset
#define READ_SCRATCH_OPCODE (0xBE)


// Datasheet: https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf
class DS18B20
{
public:

    // Constructor
    DS18B20(uint8_t pin);

    // Only function you need
    float   ReadTemperature();

private:

    // Send values to ds18b20
    void    SendBit(bool bit);
    void    SendByte(uint8_t byte);

    // Receive values from ds18b20
    bool    ReceiveBit();
    uint8_t ReceiveByte();

    // Reset bus
    bool    ResetPulse();
        
    // Convert celsius to farenheit
    float   CelsiusToFarenheit(float c);

    // Helper functions
    void    SetOutput();
    void    SetInput();
    void    SetLow();
    void    SetHigh();

    // Gpio pin
    uint8_t Pin;
};