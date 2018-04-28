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
    QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_RESET_IDX |
    QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 3999);
    //
    // Enable the quadrature encoder.
    //
    QEIEnable(QEI0_BASE);
}

uint32_t get_position(void)
{
    // Read the encoder position.
    //
    return QEIPositionGet(QEI0_BASE);
}
