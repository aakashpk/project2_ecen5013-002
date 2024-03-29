/*
 * motor_control.c
 *
 *  Created on: Apr 27, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#include "motor_control.h"

// PF0 is PWM0 GEN0, but has led3 connected
// PF1 can be used for motor control

/*
 * M0PWM0 42 PF0 (6) O TTL
 * M0PWM1 43 PF1 (6) O TTL
 */
void pwm_init(void)
{



    /*The following example shows how to use the PWM API to initialize the PWM0 with a 50 KHz
    frequency, and with a 25% duty cycle on PWM0 and a 75% duty cycle on PWM1. */
    //
    // Enable the PWM0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Setup PWM for use with PortF 0 and 1
    //GPIOPinConfigure(GPIO_PF0_M0PWM0);
    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    //GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);


    // Wait for the PWM0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0))
    { }
    //
    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    /*
     * 120 Mhz clock, div/8 makes 15Mhz in clock set. Making 15Mhz
     * 1000 ticks in
     */


    PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_8);
    // 15khz PWM with the value of 1000
    // and 1khz with 15000
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 15000);

    //
    // Set the pulse width of PWM0 for a 25% duty cycle.
    //
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 100);
    //
    // Set the pulse width of PWM1 for a 75% duty cycle.
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 500);
    //
    // Start the timers in generator 0.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    PWMOutputInvert(PWM0_BASE,(PWM_OUT_0_BIT | PWM_OUT_1_BIT),false);

    // Enable the outputs.
    //
    PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true);

}

//TODO: To be cleaned up to
void motor_speed(uint32_t duty_cycle) // value is % number and
//pulse width gets calculated in function
{
    uint32_t period, width;
    period=PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0);

    width=(duty_cycle*period/100);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width);
}

void led_bright(uint8_t value) // value is % number and
//pulse width gets calculated in function
{
    uint32_t period, width;
    period=PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0);

    width=(value*period/100);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, width);
}

void Init_PID(void)
{

    g_pid_params.kp=KP_DEFAULT_VAL;
    g_pid_params.kd=KD_DEFAULT_VAL;
    g_pid_params.ki=KI_DEFAULT_VAL;


    PID_init(&g_pid_values, g_pid_params.kp, g_pid_params.ki, g_pid_params.kd,PID_Direction_Direct); //PID_Direction_Reverse ,PID_Direction_Direct
    PID_SetOutputLimits(&g_pid_values, OUPUT_MIN_VAL, OUPUT_MAX_VAL);
    PID_SetMode(&g_pid_values, PID_Mode_Automatic);
}

uint32_t PID_compute_output(void)
{
    float speed,setpoint;

    //xSemaphoreTake( xData_Semaphore, ( TickType_t ) 10 );
    speed=gMotorValues.speed;
    setpoint=gMotorValues.setpoint;
    //xSemaphoreGive( xData_Semaphore );

    g_pid_values.myInput=speed;
    g_pid_values.mySetpoint=setpoint;
    gMotorValues.error=(setpoint-speed);

    PID_Compute(&g_pid_values);

    return (uint32_t)g_pid_values.myOutput;
    // No issues with the uint32_t cast as the limit is set for 0 to 100
}

// Functions removed we used global variable in the interest of speed
