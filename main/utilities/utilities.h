#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Most common includes
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>            // Logging
#include <driver/gpio.h>        // Gpio

#define ESP_LOG_LINE                                             \
    (printf("-----------------------------------------")

#define TASK_DELAY(n) (vTaskDelay(n / portTICK_PERIOD_MS))

// Shorter typedefs
typedef unsigned char   uchar;
typedef unsigned char   byte;
typedef unsigned int    uint;
typedef uint32_t        u32;
typedef uint16_t        u16;
typedef uint8_t         u8;

// Task creation stack sizes
#define _128B   (128)
#define _256B   (256)
#define _512B   (512)
#define _1KB    (1024)
#define _2KB    (2048)
#define _4KB    (4096)
#define _8KB    (8192)
#define _16KB   (16384)
#define _32KB   (32768)

inline int gpio_enum_to_int(gpio_num_t gpio)
{
    switch (gpio)
    {
        case GPIO_NUM_0:  return 0;
        case GPIO_NUM_1:  return 1;
        case GPIO_NUM_2:  return 2;
        case GPIO_NUM_3:  return 3;
        case GPIO_NUM_4:  return 4;
        case GPIO_NUM_5:  return 5;
        case GPIO_NUM_6:  return 6;
        case GPIO_NUM_7:  return 7;
        case GPIO_NUM_8:  return 8;
        case GPIO_NUM_9:  return 9;
        case GPIO_NUM_10: return 10;
        case GPIO_NUM_11: return 11;
        case GPIO_NUM_12: return 12;
        case GPIO_NUM_13: return 13;
        case GPIO_NUM_14: return 14;
        case GPIO_NUM_15: return 15;
        case GPIO_NUM_16: return 16;
        case GPIO_NUM_17: return 17;
        case GPIO_NUM_18: return 18;
        case GPIO_NUM_19: return 19;
        // case 20:
        case GPIO_NUM_21: return 21;
        case GPIO_NUM_22: return 22;
        case GPIO_NUM_23: return 23;
        // case 24:
        case GPIO_NUM_25: return 25;
        case GPIO_NUM_26: return 26;
        case GPIO_NUM_27: return 27;
        // case 28:
        // case 29:
        // case 30:

        default:
            ESP_LOGE("Gpio::Gpio", "Nonexistent GPIO pin, GPIO uninitialized.");
            return -1;
    }
}

inline gpio_num_t gpio_int_to_enum(int gpio)
{
    switch (gpio)
    {
        case 0:  return GPIO_NUM_0;
        case 1:  return GPIO_NUM_1;
        case 2:  return GPIO_NUM_2;
        case 3:  return GPIO_NUM_3;
        case 4:  return GPIO_NUM_4;
        case 5:  return GPIO_NUM_5;
        case 6:  return GPIO_NUM_6;
        case 7:  return GPIO_NUM_7;
        case 8:  return GPIO_NUM_8;
        case 9:  return GPIO_NUM_9;
        case 10: return GPIO_NUM_10;
        case 11: return GPIO_NUM_11;
        case 12: return GPIO_NUM_12;
        case 13: return GPIO_NUM_13;
        case 14: return GPIO_NUM_14;
        case 15: return GPIO_NUM_15;
        case 16: return GPIO_NUM_16;
        case 17: return GPIO_NUM_17;
        case 18: return GPIO_NUM_18;
        case 19: return GPIO_NUM_19;
        // case 20:
        case 21: return GPIO_NUM_21;
        case 22: return GPIO_NUM_22;
        case 23: return GPIO_NUM_23;
        // case 24:
        case 25: return GPIO_NUM_25;
        case 26: return GPIO_NUM_26;
        case 27: return GPIO_NUM_27;
        // case 28:
        // case 29:
        // case 30:

        default:
            ESP_LOGE("Gpio::Gpio", "Nonexistent GPIO pin, GPIO uninitialized.");
            return GPIO_NUM_MAX;
    }   
}

#ifdef __cplusplus
}
#endif