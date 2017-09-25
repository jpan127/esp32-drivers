#pragma once
#include "DS18B20.hpp"
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/gpio.h>

#define SENSOR_PIN (GPIO_NUM_23)

void TemperatureReadTask(void *p)
{
    DS18B20 sensor(SENSOR_PIN);

    ESP_LOGI("TemperatureReadTask", "Starting task...");

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        ESP_LOGI("TemperatureReadTask", "%f\n", sensor.ReadTemperature());
    }

    vTaskDelete(NULL);
}

float read_temperature(int n)
{
    DS18B20 sensor(SENSOR_PIN);

    float temp = 0;
    float read = 0;
    for (int i=0; i<n; i++)
    {
        read = sensor.ReadTemperature();
        // Making sure no bogus values
        if (read > 120) {
            ESP_LOGI("DS18B20", "Invalid reading %f reading again.", read);
            read = sensor.ReadTemperature();
        }
        temp += read;
        ESP_LOGI("DS18B20", "%f\n", read);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }

    return temp / n;
}