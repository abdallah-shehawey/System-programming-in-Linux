#include "uart.h"
#include "temperatureSensor.h"
#include "voltageSensor.h"

int main (void)
{
    /*For Simulation purpose */
    initAdcSimulation();
    TemperatureSensor_t* tempObjs [NUMBER_OF_TEMP_SENSOR];
    VoltageSensor_t* voltObjs [NUMBER_OF_VOLT_SENSOR];
    Adc_t* adcObjs [NUMBER_OF_ADCS];
    float sensorReading;
    int adcReading;
    tempObjs[0] = temperatureSensor_construct(ADC0_CTRL_REG, ADC0_STATUS_REG, ADC0_DATA_REG);
    adcObjs[0] = tempObjs[0]->adcParentObj;
    tempObjs[1] = temperatureSensor_construct(ADC1_CTRL_REG, ADC1_STATUS_REG, ADC1_DATA_REG);
    adcObjs[1] = tempObjs[1]->adcParentObj;
    tempObjs[2] = temperatureSensor_construct(ADC2_CTRL_REG, ADC2_STATUS_REG, ADC2_DATA_REG);
    adcObjs[2] = tempObjs[2]->adcParentObj;
    tempObjs[3] = temperatureSensor_construct(ADC3_CTRL_REG, ADC3_STATUS_REG, ADC3_DATA_REG);
    adcObjs[3] = tempObjs[3]->adcParentObj;
    tempObjs[4] = temperatureSensor_construct(ADC4_CTRL_REG, ADC4_STATUS_REG, ADC4_DATA_REG);
    adcObjs[4] = tempObjs[4]->adcParentObj;
    voltObjs[0] = voltageSensor_construct(ADC5_CTRL_REG, ADC5_STATUS_REG, ADC5_DATA_REG);
    adcObjs[5] = voltObjs[0]->adcParentObj;
    voltObjs[1] = voltageSensor_construct(ADC6_CTRL_REG, ADC6_STATUS_REG, ADC6_DATA_REG);
    adcObjs[6] = voltObjs[1]->adcParentObj;
    voltObjs[2] = voltageSensor_construct(ADC7_CTRL_REG, ADC7_STATUS_REG, ADC7_DATA_REG);
    adcObjs[7] = voltObjs[2]->adcParentObj;
    voltObjs[3] = voltageSensor_construct(ADC8_CTRL_REG, ADC8_STATUS_REG, ADC8_DATA_REG);
    adcObjs[8] = voltObjs[3]->adcParentObj;
    voltObjs[4] = voltageSensor_construct(ADC9_CTRL_REG, ADC9_STATUS_REG, ADC9_DATA_REG);
    adcObjs[9] = voltObjs[4]->adcParentObj;

    uart_init();

    for(int i = 0; i < NUMBER_OF_ADCS; i++)
    {
        adcObjs[i]->init(adcObjs[i]);
    }

    while(1)
    {
        for(int i = 0; i < NUMBER_OF_ADCS; i++)
        {
            //adcReading = adcObjs[i]->readValue(adcObjs[i]);
            //uart_transmit(&adcReading, INT);
            sensorReading = adcObjs[i]->readConvertedValue(adcObjs[i]);
            uart_transmit(&sensorReading, FLOAT);
        }
        break;
    }
    
    for(int i = 0; i < NUMBER_OF_TEMP_SENSOR; i++)
    {
        temperatureSensor_destruct(tempObjs[i]);
    }

    for(int i = 0; i < NUMBER_OF_VOLT_SENSOR; i++)
    {
        voltageSensor_destruct(voltObjs[i]);
    }
    return 0;
}