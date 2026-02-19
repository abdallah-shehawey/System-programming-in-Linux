#include "adc.h"
#include <stdio.h>
unsigned int adcSimulationMemory[NUMBER_OF_ADCS*3];

void initAdcSimulation()
{
    for(int i = 0; i < NUMBER_OF_ADCS; i++)
    {
        adcSimulationMemory[i*3] = (( i + 1 ) << 4);
        adcSimulationMemory[i*3 + 2] = i + 1;
    }
}