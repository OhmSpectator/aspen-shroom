#include <tinyprintf.h>

#include "include/serial_print.h"

static inline void wait_thr_empty(void)
{
    volatile unsigned char *uart_lsr_reg_addr = (volatile unsigned char *) (UART_BASE_ADDR + UART_LSR_REG_OFFSET);
    volatile unsigned char thr_empty = 0;
    while(thr_empty != UART_LSR_THRE_BIT)
        thr_empty = (*uart_lsr_reg_addr) & (unsigned char) UART_LSR_THRE_BIT;
}

static void local_putc(char character)
{
    volatile unsigned char *uart_thr_reg_addr = (volatile unsigned char *) (UART_BASE_ADDR + UART_THR_REG_OFFSET);
    wait_thr_empty();
    (*uart_thr_reg_addr) = (unsigned char) character;
}

static void wait_ready_for_read(void)
{
    volatile unsigned char *uart_lsr_reg_addr = (volatile unsigned char *) (UART_BASE_ADDR + UART_LSR_REG_OFFSET);
    volatile unsigned char ready_for_read = 0;
    while(ready_for_read != UART_LSR_RDR_BIT)
        ready_for_read = (*uart_lsr_reg_addr) & (unsigned char) UART_LSR_RDR_BIT;
}

static char local_getc(void)
{
    char character;
    volatile unsigned char *uart_thr_reg_addr = (volatile unsigned char *) (UART_BASE_ADDR + UART_THR_REG_OFFSET);
    wait_ready_for_read();
    character = *uart_thr_reg_addr;
    return character;
}

static void putcf_mock(void *addr, char character)
{
    (void) addr;
    local_putc(character);
}

char getc(void)
{
    return local_getc();
}

void putc(char character)
{
    local_putc(character);
}

void init_print(void)
{
    init_printf(0, putcf_mock);
}
