//
// Created by Nikolay Martyanov on 12/01/2017.
//

#include <tinyprintf.h>
#include <inttypes.h>
#include <serial_print.h>

#define MON_MODE 0b10110
#define SVC_MODE 0b10011

#define SCR_NS_BIT_OFFSET 0
#define SCR_NS_BIT (1 << SCR_NS_BIT_OFFSET)

#define SCR_HCE_BIT_OFFSET 8
#define SCR_HCE_BIT (1 << SCR_HCE_BIT_OFFSET)

#define SCR_SCD_BIT_OFFSET 7
#define SCR_SCD_BIT (1 << SCR_SCD_BIT_OFFSET)


int main(int argc, char* const argv[])
{

    init_print();
    print_magic();
    init_magic();

    print_magic();

    printf("Press any button to read ID_PFR1\n");
    (void) getc();
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
    (void) getc();
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
    (void) getc();
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
    (void) getc();
    uint32_t scr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "str r0, %0"
        : "=m" (scr_val)
        :
        : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

    printf("Press any button to switch to MON mode (via CPS)\n");
    (void) getc();
    asm volatile(
        "mov r1, sp\n"
        "mov r2, lr\n"
        "cps %0\n"
        "isb\n"
        "mov sp, r1\n"
        "mov lr, r2"
        :
        : "I" (MON_MODE)
        : "r1", "r2", "cc", "memory"
    );
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) getc();
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
    (void) getc();
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "orr r0, %0\n"
        "mcr p15, 0, r0, c1, c1, 0\n"
        "isb"
        :
        : "I" (SCR_NS_BIT)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read SCR\n");
    (void) getc();
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
    (void) getc();
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

    printf("Press any button to setup HVBAR\n");
    (void) getc();
    extern uint32_t hyp_vectors;
    uint32_t* hyp_vectors_addr = &hyp_vectors;
    asm volatile(
        "ldr r0, %0\n"
        "mcr p15, 4, r0, c12, c0, 0\n"
        :
        : "m" (hyp_vectors_addr)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read HVBAR\n");
    (void) getc();
    asm volatile(
        "mrc p15, 4, r0, c12, c0, 0\n"
        "str r0, %0"
        : "=m" (hvbar_val)
        :
        : "r0"
    );
    printf("HVBAR: 0x%08" PRIx32 "\n", hvbar_val);
    printf("Done!\n");

    printf("Press any button to set SCR.HCE and SRC.SCD to 1\n");
    (void) getc();
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "orr r0, %0\n"
        "mcr p15, 0, r0, c1, c1, 0\n"
        "isb"
        :
        : "I" (SCR_HCE_BIT | SCR_SCD_BIT)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read SCR\n");
    (void) getc();
    asm volatile(
        "mrc p15, 0, r0, c1, c1, 0\n"
        "str r0, %0"
        : "=m" (scr_val)
        :
        : "r0"
    );
    printf("SCR: 0x%08" PRIx32 "\n", scr_val);
    printf("Done!\n");

    u_int32_t local_magic = 0xABADC0DE;
    printf("Loacl magic: 0x%08X\n", local_magic);

    printf("Press any button to switch to SVC mode (via CPS)\n");
    (void) getc();
    asm volatile(
        "mov r0, sp\n"
        "mov r1, lr\n"
        "mrs r2, spsr\n"
        "cps %0\n"
        "msr spsr, r2\n"
        "isb\n"
        "mov sp, r0\n"
        "mov lr, r1\n"
        "dsb"
        :
        : "I" (SVC_MODE)
        : "r0", "r1", "sp", "cc", "memory"
    );
    init_print();
    printf("Done!\n");
    print_magic();
    printf("Loacl magic: 0x%08X\n", local_magic);

    printf("Press any button to read CPSR\n");
    (void) getc();
    asm volatile(
        "mrs r0, cpsr\n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);
    printf("Done!\n");

    printf("Press \'s\' to exit, any other button to continue\n");
    if(getc() == 's')
        return 0;

    printf("Press any button to enter HYP mode (via hvc)\n");
    (void) getc();
    asm volatile(
        "mov r0, sp\n"
        "mov r1, pc\n"
        "hvc #0\n"
        "mov sp, r0\n"
        :
        :
        : "r0", "r1"
    );
    printf("Done!\n");

    printf("Press any button to read CPSR\n");
    (void) getc();
    asm volatile(
        "mrs r0, cpsr\n"
        "str r0, %0     "
        : "=m" (cpsr_val)
        :
        : "r0"
    );
    printf("Done!\n");
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);
    (void) getc();

    return 0;
}

