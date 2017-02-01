//
// Created by Nikolay Martyanov on 12/01/2017.
//

#include "tinyprintf.h"
#include <inttypes.h>

#define MOD_MASK (~0b11111u)
#define MON_MODE 0b10110

#define UART_BASE_ADDR      0x70006300
#define UART_THR_REG_OFFSET 0x0
#define UART_LSR_REG_OFFSET 0x14
#define UART_LSR_THRE_BIT   (1<<5)
#define UART_LSR_RDR_BIT    (1<<0)


static void local_puts(const char*);
static char local_getc(void);
static void putcf_mock(void*, char);

int main(int argc, char* const argv[])
{
    init_printf(0, putcf_mock);

    printf("Press any button to read ID_PFR1\n");
    (void) local_getc();
    uint32_t id_pfr1_val;
    asm volatile(
        "mrc p15, 0, r0, c0, c1, 1\n"
        "str r0, %0"
        : "=m" (id_pfr1_val)
        :
        : "r0"
    );
    printf("ID_PFR1: 0x%08" PRIx32 "\n", id_pfr1_val);
    printf("Done!\n");

    printf("Press any button to read SCTLR\n");
    (void) local_getc();
    uint32_t sctlr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "str r0, %0"
        : "=m" (sctlr_val)
        :
        : "r0"
    );
    printf("SCTLR: 0x08%" PRIx32 "\n", sctlr_val);
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) local_getc();
    uint32_t cpsr_val;
    asm volatile (
        "mrs r0, cpsr\n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);
    printf("Done!\n");


    printf("Press any button to read SCR\n");
    (void) local_getc();
    uint32_t scr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "str r0, %0                  "
        : "=m" (scr_val)
        :
        : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

    printf("Press any button to switch to MON mode (via CPSR fix)\n");
    (void)local_getc();
    uint32_t proc_mode = MON_MODE;
    uint32_t mode_mask = MOD_MASK;
    printf("Mode mask: 0x%08" PRIx32"\n", mode_mask);
    asm volatile(
        "mrs r0, cpsr\n"
        "ldr r1, %1\n"
        "and r0, r1\n"
        "ldr r1, %0\n"
        "orr r0, r1\n"
        "mov r1, sp\n"
        "mov r2, lr\n"
        "msr cpsr, r0\n"
        "mov sp, r1\n"
        "mov lr, r2"
        :
        : "m" (proc_mode), "m" (mode_mask)
        : "r0", "r1", "r2", "cc", "memory"
    );
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) local_getc();
    asm volatile(
        "mrs r0, cpsr\n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);
    printf("Done!\n");

    printf("Press any button to set SCR.NS to 1\n");
    (void)local_getc();
    uint32_t ns_bit = 0b1;
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "ldr r1, %0\n"
        "orr r0, r1\n"
        "mcr p15, 0, r0, c1, c1, 0"
        :
        : "m" (ns_bit)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read SCR\n");
    (void) local_getc();
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "str r0, %0"
        : "=m" (scr_val)
        :
        : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

    printf("Press any button to read HVBAR\n");
    (void) local_getc();
    uint32_t hvbar_val;
    asm volatile(
        "mrc p15, 4, r0, c12, c0, 0\n"
        "str r0, %0"
        : "=m" (hvbar_val)
        :
        : "r0"
    );
    printf("HVBAR: 0x%08" PRIx32 "\n", hvbar_val);
    printf("Done!\n");

    local_puts("First test string\n");
    local_puts("Second test string\n");

    printf("Press \'s\' to exit, any other button to continue\n");
    if(local_getc() == 's')
        return 0;

    printf("Press any button to enter HYP mode (via hvc)\n");
    (void) local_getc();
    asm volatile(
        "hvc #0"
        :
        :
        :
    );
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) local_getc();
    asm volatile(
        "mrs r0, cpsr\n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("Done!\n");
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);
    (void) local_getc();

    return 0;
}

static inline void wait_thr_empty(void)
{
    volatile uint8_t* uart_lsr_reg_addr = (volatile uint8_t*)(UART_BASE_ADDR + UART_LSR_REG_OFFSET);
    volatile uint8_t thr_empty = 0;
    while(thr_empty != UART_LSR_THRE_BIT)
        thr_empty = (*uart_lsr_reg_addr) & (uint8_t)UART_LSR_THRE_BIT;
}

static void local_putc(char character)
{
    volatile uint8_t* uart_thr_reg_addr = (volatile uint8_t*)(UART_BASE_ADDR + UART_THR_REG_OFFSET);
    wait_thr_empty();
    (*uart_thr_reg_addr) = (unsigned char)character;
}

static void wait_ready_for_read(void)
{
    volatile uint8_t* uart_lsr_reg_addr = (volatile uint8_t*)(UART_BASE_ADDR + UART_LSR_REG_OFFSET);
    volatile uint8_t ready_for_read = 0;
    while(ready_for_read != UART_LSR_RDR_BIT)
        ready_for_read = (*uart_lsr_reg_addr) & (uint8_t)UART_LSR_RDR_BIT;
}

static char local_getc(void)
{
    char character;
    volatile uint8_t* uart_thr_reg_addr = (volatile uint8_t*)(UART_BASE_ADDR + UART_THR_REG_OFFSET);
    wait_ready_for_read();
    character = *uart_thr_reg_addr;
    return character;
}

static void local_puts(const char* string)
{
    const char* current_ptr = string;
    while(*current_ptr) {
        local_putc(*current_ptr);
        if(*current_ptr == '\n')
            local_putc('\r');
        current_ptr++;
    }
}

static void putcf_mock(void* addr, char character)
{
    (void)addr;
    local_putc(character);
}