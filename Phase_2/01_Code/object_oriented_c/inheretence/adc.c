#include "adc.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>

#define ADC_ENABLE_BIT  ( 1 << 0 )
#define ADC_START_BIT   ( 1 << 1 )
#define ADC_READY_BIT   ( 1 << 0 )

struct adcRegs
{
    unsigned int* ctrlRegAddr;
    unsigned int* statusRegAddr;
    unsigned int* dataRegAddr;
};

static bool isAdcConversionDone(Adc_t* obj)
{
    while(!(*obj->regs->statusRegAddr & ADC_READY_BIT)) /** Wait for conversion done */
    {
        *obj->regs->statusRegAddr  = 1;
    }
    return true;
}

static void adc_init(Adc_t* obj)
{
    *obj->regs->ctrlRegAddr |= ADC_ENABLE_BIT;
    printf("adc init 0x%X\n", *obj->regs->ctrlRegAddr);
}

static unsigned int adc_readValue(Adc_t* obj)
{
    int value = 0;

    *obj->regs->ctrlRegAddr|= ADC_START_BIT;
    while(!isAdcConversionDone(obj));
    value = *obj->regs->dataRegAddr ;
    printf("adc read Value %d\n",  value);
    return value;
}

Adc_t* adc_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr )
{
    if(ctrlRegAddr < ADC_ADDR_SPACE_START || ctrlRegAddr > ADC_ADDR_SPACE_END)
    {
        printf("Invalid control register address\nAdc object cannot be created!\n");
    }
    Adc_t* obj = (Adc_t*) malloc(sizeof(Adc_t));
    AdcRegs_t* regs = (AdcRegs_t*) malloc(sizeof(AdcRegs_t));
    regs->ctrlRegAddr   = ctrlRegAddr;
    regs->statusRegAddr = statusRegAddr;
    regs->dataRegAddr   = dataRegAddr;
    obj->regs           = regs;
    obj->init           = adc_init;
    obj->readValue      = adc_readValue;
    printf("ADC object is created %d\n",*dataRegAddr);
    return obj;
}

void adc_destruct(Adc_t* obj)
{
    free(obj->regs);
    free(obj);
}