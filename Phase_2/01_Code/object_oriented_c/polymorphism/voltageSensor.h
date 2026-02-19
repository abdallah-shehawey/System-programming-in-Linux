#ifndef VOLT_SENSOR
#define VOLT_SENSOR
#include "adc.h"

#define NUMBER_OF_VOLT_SENSOR 5

typedef struct voltageSensor VoltageSensor_t;

struct voltageSensor{
    Adc_t* adcParentObj;
};

VoltageSensor_t* voltageSensor_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr );

void voltageSensor_destruct(VoltageSensor_t* obj);
#endif