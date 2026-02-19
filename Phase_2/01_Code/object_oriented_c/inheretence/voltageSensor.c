#include "voltageSensor.h"
#include <stdlib.h>
#include <stdio.h>

static float readConvertedVoltageValue(VoltageSensor_t* obj)
{
    return (obj->adcParentObj->readValue(obj->adcParentObj) / 1000.0);
}

VoltageSensor_t* voltageSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr )
{
    VoltageSensor_t* obj = (VoltageSensor_t*) malloc(sizeof(VoltageSensor_t)); 
    obj->adcParentObj = adc_construct(ctrlRegAddr, statusRegAddr, dataRegAddr);
    obj->readConvertedValue = readConvertedVoltageValue;
    printf("volt object is created %d\n",*dataRegAddr);
    return obj;
}

void voltageSensor_destruct(VoltageSensor_t* obj)
{
    adc_destruct(obj->adcParentObj);
    free(obj);
}