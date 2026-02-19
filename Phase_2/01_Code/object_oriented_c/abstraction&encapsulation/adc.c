#include "adc.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>

#define ADC_ENABLE_BIT  ( 1 << 0 )
#define ADC_START_BIT   ( 1 << 1 )
#define ADC_READY_BIT   ( 1 << 0 )
/**This struct  "adcRegs" is to implement the encapsulation, 
*this file can be compiled standalone and only the header file is visible to the user */
struct adcRegs
{
    unsigned int* ctrlRegAddr;
    unsigned int* statusRegAddr;
    unsigned int* dataRegAddr;
};
/** This function is considered private as it is static in the file and has no pointer in the struct */
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

void setCtrlAddress(Adc_t* obj, unsigned int* ctrlRegAddr)
{
     if(ctrlRegAddr < ADC_ADDR_SPACE_START || ctrlRegAddr > ADC_ADDR_SPACE_END)
    {
        printf("Invalid control register address\nRegister is not set!\n");
        return;
    }
    obj->regs->ctrlRegAddr = ctrlRegAddr;
}

void setStatusAddress(Adc_t* obj, unsigned int* statusRegAddr)
{
     if(statusRegAddr < ADC_ADDR_SPACE_START || statusRegAddr > ADC_ADDR_SPACE_END)
    {
        printf("Invalid status register address\nRegister is not set!\n");
        return;
    }
    obj->regs->statusRegAddr = statusRegAddr;
}

void setDataAddress(Adc_t* obj, unsigned int* dataRegAddr)
{
     if(dataRegAddr < ADC_ADDR_SPACE_START || dataRegAddr > ADC_ADDR_SPACE_END)
    {
        printf("Invalid data register address\nRegister is not set!\n");
        return;
    }
    obj->regs->dataRegAddr = dataRegAddr;
}

static unsigned int* getCtrlAddress(Adc_t* obj)
{
    return obj->regs->ctrlRegAddr;
}

static unsigned int* getDataAddress(Adc_t* obj)
{
    return obj->regs->dataRegAddr;
}

static unsigned int* getStatusAddress(Adc_t* obj)
{
    return obj->regs->statusRegAddr;
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
    obj->setCtrlAddress = setCtrlAddress;
    obj->setDataAddress = setDataAddress;
    obj->setStatusAddress = setStatusAddress;
    obj->getCtrlAddress = getCtrlAddress;
    obj->getDataAddress = getDataAddress;
    obj->getStatusAddress = getStatusAddress;
    return obj;
}

void adc_destruct(Adc_t* obj)
{
    free(obj->regs);
    free(obj);
}


