#ifndef TEMP_SENSOR
#define TEMP_SENSOR
#include "adc.h"

#define NUMBER_OF_TEMP_SENSOR 5

typedef struct temperatureSensor TemperatureSensor_t;

struct temperatureSensor{
    Adc_t* adcParentObj;
};

TemperatureSensor_t* temperatureSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr );

void temperatureSensor_destruct(TemperatureSensor_t* obj);
#endif