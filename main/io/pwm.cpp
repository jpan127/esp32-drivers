#include "pwm.hpp"

Pwm::Pwm(mcpwm_unit_t pwm_unit, int gpio_a, int gpio_b, mcpwm_timer_t timer)
{
    PwmUnit  = pwm_unit;
    GpioA    = gpio_a;
    GpioB    = gpio_b;
    PwmTimer = timer;

    // Pwm default config
    PwmConfig.frequency    = 1000;
    PwmConfig.cmpr_a       = 0;
    PwmConfig.cmpr_b       = 0;
    PwmConfig.duty_mode    = MCPWM_DUTY_MODE_0;     // Active high duty
    PwmConfig.counter_mode = MCPWM_UP_COUNTER;
}

Pwm::Pwm(mcpwm_unit_t pwm_unit, int gpio, mcpwm_timer_t timer)
{
    PwmUnit  = pwm_unit;
    GpioA    = gpio;
    GpioB    = DISABLED_PWM;
    PwmTimer = timer;

    // Pwm default config
    PwmConfig.frequency    = 1000;
    PwmConfig.cmpr_a       = 0;
    PwmConfig.cmpr_b       = 0;
    PwmConfig.duty_mode    = MCPWM_DUTY_MODE_0;     // Active high duty
    PwmConfig.counter_mode = MCPWM_UP_COUNTER;
}

void Pwm::Initialize()
{
    static const char *TAG = "Pwm::Initialize";

    mcpwm_io_signals_t io_a;
    mcpwm_io_signals_t io_b;
    switch (PwmTimer)
    {
        case MCPWM_TIMER_0:
            io_a = MCPWM0A;
            io_b = MCPWM0B;
            break;
        case MCPWM_TIMER_1:
            io_a = MCPWM1A;
            io_b = MCPWM1B;
            break;
        case MCPWM_TIMER_2:
            io_a = MCPWM2A;
            io_b = MCPWM2B;
            break;
        default: 
            break;
    }

    // Initialize GPIO
    ESP_ERROR_CHECK(mcpwm_gpio_init(PwmUnit, io_a, GpioA));
    if (GpioB != DISABLED_PWM) {
        ESP_ERROR_CHECK(mcpwm_gpio_init(PwmUnit, io_b, GpioB))        
    }
    ESP_LOGI(TAG, "Initialized GPIO.");

    // Initialize PWM
    ESP_ERROR_CHECK(mcpwm_init(PwmUnit, PwmTimer, &PwmConfig));
    ESP_LOGI(TAG, "Initialized PWM Configuration.");
}

void Pwm::SetTimer(mcpwm_timer_t timer)
{
    PwmTimer = timer;
    ESP_LOGI("Pwm::SetTimer", "Changed PWM Timer.");
}

void Pwm::SetFrequency(uint32_t frequency)
{
    PwmConfig.frequency = frequency;
    ESP_LOGI("Pwm::SetFrequency", "Changed PWM Frequency.");
}

void Pwm::SetDuty(float duty_a)
{
    PwmConfig.cmpr_a = duty_a;
    ESP_LOGI("Pwm::SetDuty", "Changed duty to %f.", duty_a);
}

void Pwm::SetDuty(float duty_a, float duty_b)
{
    PwmConfig.cmpr_a = duty_a;
    if (GpioB != DISABLED_PWM) {
        PwmConfig.cmpr_b = duty_b;        
    }
    ESP_LOGI("Pwm::SetDuty", "Changed duty to %f / %f.", duty_a, duty_b);
}

void Pwm::Start()
{
    ESP_ERROR_CHECK(mcpwm_start(PwmUnit, PwmTimer));
    ESP_LOGI("Pwm::Start", "Starting PWM.");
}

void Pwm::GeneratePwm(pwm_output_t pwm_output, float duty)
{
    static const char *TAG = "Pwm::GeneratePwm";

    if (pwm_output == PWM_B && GpioB == DISABLED_PWM) {
        ESP_LOGE(TAG, "Cannot generate PWM for GPIO B when it is not initialized.");
        return;
    }

    switch (pwm_output)
    {
        // Only for MCPWMXA
        case PWM_A:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_A));
            ESP_ERROR_CHECK(mcpwm_set_duty(PwmUnit, PwmTimer, MCPWM_OPR_A, duty));
            ESP_LOGI(TAG, "Generating MCPWMXA : %f", duty);
            break;
        // Only for MCPWMXB
        case PWM_B:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_B));
            ESP_ERROR_CHECK(mcpwm_set_duty(PwmUnit, PwmTimer, MCPWM_OPR_B, duty));
            ESP_LOGI(TAG, "Generating MCPWMXB : %f", duty);
            break;
        // Both MCPWMXA and MCPWMXB
        case PWM_AB:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_A));
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_B));
            ESP_ERROR_CHECK(mcpwm_set_duty(PwmUnit, PwmTimer, MCPWM_OPR_A, duty));
            ESP_ERROR_CHECK(mcpwm_set_duty(PwmUnit, PwmTimer, MCPWM_OPR_B, duty));
            ESP_LOGI(TAG, "Generating MCPWMXA + MCPWMXB : %f", duty);
            break;
    }
}

void Pwm::Stop(pwm_output_t pwm_output)
{
    static const char *TAG = "Pwm::Stop";

    if (pwm_output == PWM_B && GpioB == DISABLED_PWM) {
        ESP_LOGE(TAG, "Cannot generate PWM for GPIO B when it is not initialized.");
        return;
    }

    switch (pwm_output)
    {
        case PWM_A:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_A));
            ESP_LOGI(TAG, "Stopping MCPWMXA.");
            break;
        case PWM_B:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_B));
            ESP_LOGI(TAG, "Stopping MCPWMXB.");
            break;
        case PWM_AB:
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_A));
            ESP_ERROR_CHECK(mcpwm_set_signal_low(PwmUnit, PwmTimer, MCPWM_OPR_B));
            ESP_LOGI(TAG, "Stopping MCPWMXA + MCPWMXB.");
            break;
    }
}