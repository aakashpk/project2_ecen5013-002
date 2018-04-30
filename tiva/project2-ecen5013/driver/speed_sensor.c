/*
 * speed_sensor.c
 *
 *  Created on: Apr 26, 2018
 *      Author: Aakash
 *      Author: Miles
 */

#include "speed_sensor.h"

void quad_encoder_init(void)
{
    /* QEI is on Port L 1,2,3
    IDX0 84 PL3 (6) I TTL QEI module 0 index.
    PhA0 82 PL1 (6) I TTL QEI module 0 phase A.
    PhB0 83 PL2 (6) I TTL QEI module 0 phase B.
    */

    //
    // Enable the QEI0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);



    //
    // Wait for the QEI0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0))
    { }

    // Setup QEI with Port L
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    GPIOPinConfigure(GPIO_PL1_PHA0);
    GPIOPinConfigure(GPIO_PL2_PHB0);
    GPIOPinTypeQEI(GPIO_PORTL_BASE,GPIO_PIN_1 | GPIO_PIN_2);


    //
    // Configure the quadrature encoder to capture edges on both signals and
    // maintain an absolute position by resetting on index pulses. Using a
    // 1000 line encoder at four edges per line, there are 4000 pulses per
    // revolution; therefore set the maximum position to 3999 as the count
    // is zero based.
    //


    // count set to 63 as 64 pulses per revolution
    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
    QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 63);
    //
    // Enable the quadrature encoder.
    //
    QEIEnable(QEI0_BASE);

    QEIVelocityConfigure(QEI0_BASE,QEI_VELDIV_4,480000);

    QEIVelocityEnable(QEI0_BASE);
}

uint32_t get_position(void)
{
    // Read the encoder position.
    //
    return QEIPositionGet(QEI0_BASE);
}

void tachometer_init(void)
{
    // Tachometer connected to PP3

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_3);

    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //GPIODirModeSet(GPIO_PORTN_BASE,GPIO_PIN_4,GPIO_DIR_MODE_HW);

    //GPIOIntClear(GPIO_PORTP_BASE,GPIO_INT_PIN_3);

    GPIOIntTypeSet(GPIO_PORTP_BASE,GPIO_PIN_3,GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTP_BASE,GPIOPP3IntHandler);
    GPIOIntEnable(GPIO_PORTP_BASE,GPIO_INT_PIN_3);

}

static int a=0;

void GPIOPP3IntHandler(void)
{
    GPIOIntClear(GPIO_PORTP_BASE,GPIO_INT_PIN_3);
    timer_past_val=timer_cur_val;
    timer_cur_val = TimerValueGet();

}

uint64_t timer_cur_val, timer_past_val;

void timer_init(void)
{

}

float get_speed_2(void)
{
    return timer_cur_val-timer_past_val;

}

float get_speed(void)
{
   float value=0;

   value=(QEIVelocityGet(QEI0_BASE)*PERIODS_PER_SEC/PULSES_PER_REV);

   return value;
}

uint32_t get_speed_test(void)
{
   uint32_t value=0;

   value=QEIVelocityGet(QEI0_BASE);

   return value;
}

float get_speed_setpoint(void)
{
   float value=0;

   return value;
}

float get_error(void)
{
   float value=0;

   return value;
}





