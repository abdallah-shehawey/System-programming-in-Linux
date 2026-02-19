#include "voltageSensor.h"
#include <stdlib.h>

static float readConvertedVoltageValue(Adc_t* obj)
{
    return (obj->readValue(obj) / 1000.0);
}

VoltageSensor_t* voltageSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr )
{
    VoltageSensor_t* obj = (VoltageSensor_t*) malloc(sizeof(VoltageSensor_t)); 
    obj->adcParentObj = adc_construct(ctrlRegAddr, statusRegAddr, dataRegAddr, 
            readConvertedVoltageValue);
    return obj;
}

void voltageSensor_destruct(VoltageSensor_t* obj)
{
    adc_destruct(obj->adcParentObj);
    free(obj);
}