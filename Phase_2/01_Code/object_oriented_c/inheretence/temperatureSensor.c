#include "temperatureSensor.h"
#include <stdlib.h>
#include <stdio.h>

static float readConvertedTemperatureValue(TemperatureSensor_t* obj)
{
    return (obj->adcParentObj->readValue(obj->adcParentObj) * 9/5) + 32;
}

TemperatureSensor_t* temperatureSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr )
{
    TemperatureSensor_t* obj = (TemperatureSensor_t*) malloc(sizeof(TemperatureSensor_t)); 
    obj->adcParentObj = adc_construct(ctrlRegAddr, statusRegAddr, dataRegAddr);
    obj->readConvertedValue = readConvertedTemperatureValue;
    printf("temp object is created %d\n",*dataRegAddr);
    return obj;
}

void temperatureSensor_destruct(TemperatureSensor_t* obj)
{
    adc_destruct(obj->adcParentObj);
    free(obj);
}