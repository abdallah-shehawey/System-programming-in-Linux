#define NUMBER_OF_ADCS          10

#include "adcSimulation.h"

#define ADC0_CTRL_REG    ((unsigned int*)&adcSimulationMemory[0])
#define ADC0_STATUS_REG  ((unsigned int*)&adcSimulationMemory[1])
#define ADC0_DATA_REG    ((unsigned int*)&adcSimulationMemory[2])

#define ADC1_CTRL_REG    ((unsigned int*)&adcSimulationMemory[3])
#define ADC1_STATUS_REG  ((unsigned int*)&adcSimulationMemory[4])
#define ADC1_DATA_REG    ((unsigned int*)&adcSimulationMemory[5])

#define ADC2_CTRL_REG    ((unsigned int*)&adcSimulationMemory[6])
#define ADC2_STATUS_REG  ((unsigned int*)&adcSimulationMemory[7])
#define ADC2_DATA_REG    ((unsigned int*)&adcSimulationMemory[8])

#define ADC3_CTRL_REG    ((unsigned int*)&adcSimulationMemory[9])
#define ADC3_STATUS_REG  ((unsigned int*)&adcSimulationMemory[10])
#define ADC3_DATA_REG    ((unsigned int*)&adcSimulationMemory[11])

#define ADC4_CTRL_REG    ((unsigned int*)&adcSimulationMemory[12])
#define ADC4_STATUS_REG  ((unsigned int*)&adcSimulationMemory[13])
#define ADC4_DATA_REG    ((unsigned int*)&adcSimulationMemory[14])

#define ADC5_CTRL_REG    ((unsigned int*)&adcSimulationMemory[15])
#define ADC5_STATUS_REG  ((unsigned int*)&adcSimulationMemory[16])
#define ADC5_DATA_REG    ((unsigned int*)&adcSimulationMemory[17])

#define ADC6_CTRL_REG    ((unsigned int*)&adcSimulationMemory[18])
#define ADC6_STATUS_REG  ((unsigned int*)&adcSimulationMemory[10])
#define ADC6_DATA_REG    ((unsigned int*)&adcSimulationMemory[20])

#define ADC7_CTRL_REG    ((unsigned int*)&adcSimulationMemory[21])
#define ADC7_STATUS_REG  ((unsigned int*)&adcSimulationMemory[22])
#define ADC7_DATA_REG    ((unsigned int*)&adcSimulationMemory[23])

#define ADC8_CTRL_REG    ((unsigned int*)&adcSimulationMemory[24])
#define ADC8_STATUS_REG  ((unsigned int*)&adcSimulationMemory[25])
#define ADC8_DATA_REG    ((unsigned int*)&adcSimulationMemory[26])

#define ADC9_CTRL_REG    ((unsigned int*)&adcSimulationMemory[27])
#define ADC9_STATUS_REG  ((unsigned int*)&adcSimulationMemory[28])
#define ADC9_DATA_REG    ((unsigned int*)&adcSimulationMemory[29])


#define ADC_ADDR_SPACE_START ADC0_CTRL_REG
#define ADC_ADDR_SPACE_END   ADC9_DATA_REG

typedef struct adc Adc_t;

typedef struct adcRegs AdcRegs_t;

struct adc{
    AdcRegs_t* regs;
    void (*init)(Adc_t* obj);
    unsigned int (*readValue)(Adc_t* obj);
    void (*setCtrlAddress)(Adc_t* obj, unsigned int* ctrlRegAddr);
    void (*setStatusAddress)(Adc_t* obj, unsigned int* statusRegAddr);
    void (*setDataAddress)(Adc_t* obj, unsigned int* dataRegAddr);
    unsigned int* (*getCtrlAddress)(Adc_t* obj);
    unsigned int* (*getStatusAddress)(Adc_t* obj);
    unsigned int* (*getDataAddress)(Adc_t* obj);
};

Adc_t* adc_construct( unsigned int* ctrlRegAddr, 
                unsigned int* statusRegAddr, 
                unsigned int* dataRegAddr );

void adc_destruct(Adc_t* obj);

