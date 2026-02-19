#define UART0_CTRL_REG    0x40020000
#define UART0_STATUS_REG  0x40020004
#define UART0_DATA_REG    0x40020008

#define UART1_CTRL_REG    0x40021000
#define UART1_STATUS_REG  0x40021004
#define UART1_DATA_REG    0x40021008

#define UART2_CTRL_REG    0x40022000
#define UART2_STATUS_REG  0x40022004
#define UART2_DATA_REG    0x40022008

#define UART3_CTRL_REG    0x40023000
#define UART3_STATUS_REG  0x40023004
#define UART3_DATA_REG    0x40023008

void uart_init();
void uart_transmit(unsigned int value);
void uart_transmitFloat(float value);
