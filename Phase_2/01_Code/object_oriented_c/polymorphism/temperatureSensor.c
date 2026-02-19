#include "temperatureSensor.h"
#include <stdlib.h>

static float readConvertedTemperatureValue(Adc_t* obj)
{
    return (obj->readValue(obj) * 9/5) + 32;
}

TemperatureSensor_t* temperatureSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr )
{
    TemperatureSensor_t* obj = (TemperatureSensor_t*) malloc(sizeof(TemperatureSensor_t)); 
    obj->adcParentObj = adc_construct(ctrlRegAddr, statusRegAddr, dataRegAddr,
                            readConvertedTemperatureValue);
    return obj;
}

void temperatureSensor_destruct(TemperatureSensor_t* obj)
{
    adc_destruct(obj->adcParentObj);
    free(obj);
}