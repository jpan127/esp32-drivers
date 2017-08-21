#pragma once
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rom/gpio.h"

#define BLINK_GPIO GPIO_NUM_23

void blink_task(void *pvParameter);