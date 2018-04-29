/*
 * current_sensor.c
 *
 *  Created on: Apr 26, 2018
 *      Author: Aakash
 */

#include "driver/current_sensor.h"


void adc_init(void)
{
    //
    // Enable the ADC0 module.
    //Comes out on PE3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 );

    //
    // Wait for the ADC0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    { }


    //
    // Enable the first sample sequencer to capture the value of channel 0 when
    // the processor trigger occurs.
    //
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
    ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE, 0);


}

/*
 * SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 );

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 1);

    while(1)
    {
        ADCIntClear(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 1);
        while(!ADCIntStatus(ADC0_BASE, 1, false))
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 1, ui32ACCValues);
        ui32AccX = ui32ACCValues[0];
        ui32AccY = ui32ACCValues[1];
        ui32AccZ = ui32ACCValues[2];
    }
 */

uint32_t get_adc_count(void)
{
    uint32_t value=5432;

    // Trigger the sample sequence.
    ADCProcessorTrigger(ADC0_BASE, 0);

    //
    // Wait until the sample sequence has completed.
    //
    while(!ADCIntStatus(ADC0_BASE, 0, false))
    { }

    // Read the value from the ADC.
    //
    ADCSequenceDataGet(ADC0_BASE, 0, &value);

    return value;

}


float get_current(void)
{
    uint32_t current=0;

    // Trigger the sample sequence.
    ADCProcessorTrigger(ADC0_BASE, 0);

    //
    // Wait until the sample sequence has completed.
    //
    while(!ADCIntStatus(ADC0_BASE, 0, false))
    { }

    // Read the value from the ADC.
    //
    ADCSequenceDataGet(ADC0_BASE, 0, &current);

    return (current*ADC_FACTOR/CURRENT_MEASURE_RESISTANCE);

}
