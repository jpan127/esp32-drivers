#include "motor.hpp"

MotorDriver::MotorDriver(int enableA, int enableB, 
                        int pwmA,     int pwmB, 
                        mcpwm_unit_t pwm_unit, mcpwm_timer_t timer) :
                        EnableA(enableA),                       // Constructor for member GpioOutput::EnableA
                        EnableB(enableB),                       // Constructor for member GpioOutput::EnableB
                        Motors(pwm_unit, pwmA, pwmB, timer)     // Constructor for member Pwm::Motors
{
    // Configure motors and start
    Motors.Initialize();
    Motors.Start();
}

void MotorDriver::Forward(float duty)
{
    EnableA.Set();
    EnableB.Set();
    Motors.GeneratePwm(PWM_AB, duty);
}

void MotorDriver::Backward(float duty)
{
    EnableA.Clear();
    EnableB.Clear();
    Motors.GeneratePwm(PWM_AB, duty);
}

void MotorDriver::Stop()
{
    Motors.GeneratePwm(PWM_AB, (float)0);
}

void MotorDriver::Left(float duty)
{
    EnableA.Clear();
    EnableB.Set();
    Motors.GeneratePwm(PWM_B, duty);
    Motors.GeneratePwm(PWM_A, 0);
}

void MotorDriver::Right(float duty)
{
    EnableA.Set();
    EnableB.Clear();
    Motors.GeneratePwm(PWM_A, duty);
    Motors.GeneratePwm(PWM_B, 0);
}