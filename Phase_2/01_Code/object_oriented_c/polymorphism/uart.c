#include "uart.h"
#include <stdio.h>

void uart_init()
{

}

void uart_transmit(void* value, EnumValueType_e type)
{
    switch(type)
    {
        case CHAR:
            printf("uart transmit %c\n",*((char*)value));
        break;
        case INT:
            printf("uart transmit %d\n",*((int*)value));
        break;
        case FLOAT:
            printf("uart transmit %f\n",*((float*)value));
        break;
    }
    
    
}

void uart_transmitFloat(float value)
{
    printf("uart transmit %f\n",value);
}
