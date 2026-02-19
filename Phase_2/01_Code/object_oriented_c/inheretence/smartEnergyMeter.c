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
    int   adcReading;
    tempObjs[0] = temperatureSensor_construct(ADC0_CTRL_REG, ADC0_STATUS_REG, ADC0_DATA_REG);
    tempObjs[1] = temperatureSensor_construct(ADC1_CTRL_REG, ADC1_STATUS_REG, ADC1_DATA_REG);
    tempObjs[2] = temperatureSensor_construct(ADC2_CTRL_REG, ADC2_STATUS_REG, ADC2_DATA_REG);
    tempObjs[3] = temperatureSensor_construct(ADC3_CTRL_REG, ADC3_STATUS_REG, ADC3_DATA_REG);
    tempObjs[4] = temperatureSensor_construct(ADC4_CTRL_REG, ADC4_STATUS_REG, ADC4_DATA_REG);
    voltObjs[0] = voltageSensor_construct(ADC5_CTRL_REG, ADC5_STATUS_REG, ADC5_DATA_REG);
    voltObjs[1] = voltageSensor_construct(ADC6_CTRL_REG, ADC6_STATUS_REG, ADC6_DATA_REG);
    voltObjs[2] = voltageSensor_construct(ADC7_CTRL_REG, ADC7_STATUS_REG, ADC7_DATA_REG);
    voltObjs[3] = voltageSensor_construct(ADC8_CTRL_REG, ADC8_STATUS_REG, ADC8_DATA_REG);
    voltObjs[4] = voltageSensor_construct(ADC9_CTRL_REG, ADC9_STATUS_REG, ADC9_DATA_REG);
    uart_init();

    for(int i = 0; i < NUMBER_OF_TEMP_SENSOR; i++)
    {
        tempObjs[i]->adcParentObj->init(tempObjs[i]->adcParentObj);
    }

    for(int i = 0; i < NUMBER_OF_VOLT_SENSOR; i++)
    {
        voltObjs[i]->adcParentObj->init(voltObjs[i]->adcParentObj);
    }

    while(1)
    {

        for(int i = 0; i < NUMBER_OF_TEMP_SENSOR; i++)
        {
            adcReading = tempObjs[i]->adcParentObj->readValue(tempObjs[i]->adcParentObj);
            //uart_transmit(adcReading);
            sensorReading = tempObjs[i]->readConvertedValue(tempObjs[i]);
            uart_transmitFloat(sensorReading);
        }

        for(int i = 0; i < NUMBER_OF_VOLT_SENSOR; i++)
        {
            adcReading = voltObjs[i]->adcParentObj->readValue(voltObjs[i]->adcParentObj);
            //uart_transmit(adcReading);
            sensorReading = voltObjs[i]->readConvertedValue(voltObjs[i]);
            uart_transmitFloat(sensorReading);
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