#include "adc.h"
#include "uart.h"
#include <stdio.h>

int main (void)
{
    /*For Simulation purpose */
    initAdcSimulation();
    Adc_t* adcObjs [NUMBER_OF_ADCS];
    adcObjs[0] = adc_construct(ADC0_CTRL_REG, ADC0_STATUS_REG, ADC0_DATA_REG);
    adcObjs[1] = adc_construct(ADC1_CTRL_REG, ADC1_STATUS_REG, ADC1_DATA_REG);
    adcObjs[2] = adc_construct(ADC2_CTRL_REG, ADC2_STATUS_REG, ADC2_DATA_REG);
    adcObjs[3] = adc_construct(ADC3_CTRL_REG, ADC3_STATUS_REG, ADC3_DATA_REG);
    adcObjs[4] = adc_construct(ADC4_CTRL_REG, ADC4_STATUS_REG, ADC4_DATA_REG);
    adcObjs[5] = adc_construct(ADC5_CTRL_REG, ADC5_STATUS_REG, ADC5_DATA_REG);
    adcObjs[6] = adc_construct(ADC6_CTRL_REG, ADC6_STATUS_REG, ADC6_DATA_REG);
    adcObjs[7] = adc_construct(ADC7_CTRL_REG, ADC7_STATUS_REG, ADC7_DATA_REG);
    adcObjs[8] = adc_construct(ADC8_CTRL_REG, ADC8_STATUS_REG, ADC8_DATA_REG);
    adcObjs[9] = adc_construct(ADC9_CTRL_REG, ADC9_STATUS_REG, ADC9_DATA_REG);
    //const Adc_t* trialInvalidAdc = adc_construct(ADC0_CTRL_REG, ADC0_STATUS_REG, ADC0_DATA_REG);
    uart_init();

    for(int i = 0; i < NUMBER_OF_ADCS; i++)
    {
        adcObjs[i]->init(adcObjs[i]);
    }
    while(1)
    {
        for(int i = 0; i < NUMBER_OF_ADCS; i++)
        {
            unsigned int adcValue = adcObjs[i]->readValue(adcObjs[i]);
            uart_transmit(adcValue);
        }
        break;
    }
    /**Try setter and getter functions */
    printf("get ctrlAddr1 %x\n",(int)adcObjs[0]->getCtrlAddress(adcObjs[0]));
    adcObjs[0]->setCtrlAddress(adcObjs[0], ADC7_CTRL_REG);
    printf("get ctrlAddr1 %x\n",(int)adcObjs[0]->getCtrlAddress(adcObjs[0]));
    adcObjs[0]->setDataAddress(adcObjs[0], ADC7_DATA_REG);
    adcObjs[0]->readValue(adcObjs[0]);

    for(int i = 0; i < NUMBER_OF_ADCS; i++)
    {
        adc_destruct(adcObjs[i]);
    }
    return 0;
}