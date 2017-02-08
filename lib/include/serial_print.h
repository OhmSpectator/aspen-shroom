#ifndef ASPEN_SHROOM_SERIAL_PRINT_H
#define ASPEN_SHROOM_SERIAL_PRINT_H

#define UART_BASE_ADDR      0x70006300
#define UART_THR_REG_OFFSET 0x0
#define UART_LSR_REG_OFFSET 0x14
#define UART_LSR_THRE_BIT   (1<<5)
#define UART_LSR_RDR_BIT    (1<<0)

void init_print(void);

char getc(void);
void putc(char);

#endif //ASPEN_SHROOM_SERIAL_PRINT_H
