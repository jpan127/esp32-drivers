#include "servo.hpp"

Servo::Servo(mcpwm_unit_t pwm_unit, pin_t pwm_pin) : Pwm(pwm_unit, pwm_pin)
{
    SetFrequency(FREQUENCY);
    Initialize();
    Start();
}

float Servo::PositionToDutyCycle(int8_t position)
{
    // Make sure position stays in bounds
    if      (position < -90) position = -90;
    else if (position > 90)  position = 90;

    // -90 is a 1ms high / 20 ms = 0.05 duty cycle
    // +90 is a 2ms high / 20 ms = 0.10 duty cycle
    // Map 180 to 0.05

    // Absolute value position from [-90, 90] to [0, 180]
    position                += 90;
    const float cycle_range = 0.05;
    const float pos_range   = 180;
    float offset            = (float)position * (cycle_range / pos_range);
    float duty_cycle        = 0.05 + offset;

    return duty_cycle;
}

void Servo::SetPosition(int8_t position)
{
    SetDuty(PositionToDutyCycle(position));
}

void Servo::SetMaxPositive()
{
    SetPosition(90);
}

void Servo::SetStraight()
{
    SetPosition(0);
}

void Servo::SetMaxNegative()
{
    SetPosition(-90);
}

// void Servo::Sweep(servo_sweep_speed_t speed, uint32_t milliseconds)
// {
//     switch (speed)
//     {
//         case SWEEP_SLOW:    break;
//         case SWEEP_NORMAL:  break;
//         case SWEEP_FAST:    break;
//     }
// }