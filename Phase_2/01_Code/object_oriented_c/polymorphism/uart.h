typedef enum valueType{
    CHAR,
    INT,
    FLOAT
}EnumValueType_e;

void uart_init();
void uart_transmit(void* value, EnumValueType_e type);