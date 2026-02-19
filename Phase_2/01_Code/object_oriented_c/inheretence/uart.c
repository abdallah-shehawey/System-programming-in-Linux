#include "uart.h"
#include <stdio.h>

void uart_init()
{

}

void uart_transmit(unsigned int value)
{
    printf("uart transmit %d\n",value);
}

void uart_transmitFloat(float value)
{
    printf("uart transmit %f\n",value);
}
