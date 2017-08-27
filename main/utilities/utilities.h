#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Most common includes
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>            // Logging

#define ESP_LOG_LINE                                             \
    (printf("-----------------------------------------")

#define TASK_DELAY(n) (vTaskDelay(n / portTICK_PERIOD_MS))

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

#ifdef __cplusplus
}
#endif