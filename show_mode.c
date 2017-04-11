//
// Created by Nikolay Martyanov on 12/01/2017.
//

#include <tinyprintf.h>
#include <inttypes.h>
#include <serial_print.h>

#define MON_MODE 0b10110
#define SVC_MODE 0b10011

#define SCR_NS_BIT_OFFSET 0
#define SCR_NS_BIT (1U << SCR_NS_BIT_OFFSET)

#define SCR_HCE_BIT_OFFSET 8
#define SCR_HCE_BIT (1U << SCR_HCE_BIT_OFFSET)

#define SCR_SCD_BIT_OFFSET 7
#define SCR_SCD_BIT (1U << SCR_SCD_BIT_OFFSET)

#define SCTLR_C_BIT_OFFSET 2
#define SCTLR_C_BIT (1U << SCTLR_C_BIT_OFFSET)


int main(int argc, char* const argv[])
{

    init_print();

    print_magic();
    init_magic();
    print_magic();

    printf("Disable data caches...\n");
    (void) getc();
    volatile u_int32_t sctlr_val;
    u_int32_t sctlr_c_bit = ~SCTLR_C_BIT;
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "ldr r1, %0\n"
        "and r0, r1\n"
        "mcr p15, 0, r0, c1, c0, 0\n"
        "isb"
        :
        : "m" (sctlr_c_bit)
        : "r0", "r1"
    );
    printf("Done!\n");

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

    printf("Press any button to read SCTLR\n");
    (void) getc();
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "str r0, %0"
        : "=m" (sctlr_val)
        :
        : "r0"
    );
    printf("SCTLR: 0x08%" PRIx32 " (@0x%08" PRIx32 ")\n", sctlr_val, (u_int32_t)&sctlr_val);

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
        : "r1", "r2", "r3", "cc", "memory"
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

    printf("Press \'s\' to exit, any other button to continue\n");
    if(getc() == 's')
        return 0;

    printf("Press any button to read TTBCR\n");
    (void)getc();
    u_int32_t ttbcr_val;
    asm volatile(
        "mrc p15, 0, r0, c2, c0, 2\n"
        "str r0, %0"
        : "=m" (ttbcr_val)
        :
        : "r0"
    );
    printf("TTBCR: 0x%08" PRIx32 "\n", ttbcr_val);

    printf("Press any button to read TTBR0\n");
    (void)getc();
    u_int32_t ttbr0_low_val, ttbr0_high_val;
    asm volatile(
        "mrrc p15, 0, r0, r1, c2\n"
        "str r0, %0\n"
        "str r1, %1\n"
        : "=m" (ttbr0_low_val), "=m" (ttbr0_high_val)
        :
        : "r0", "r1"
    );
    printf("TTBR0: low - 0x%08" PRIx32 ", high - 0x%08" PRIx32 "\n", ttbr0_low_val, ttbr0_high_val);

    volatile u_int32_t local_magic = 0xABADC0DE;
    printf("Local magic: 0x%08X\n", local_magic);

    printf("Press any button to switch to non-secure SVC mode (via CPS)\n");
    (void) getc();
    asm volatile(
        "dsb\n"
        "dmb\n"
        "mov r0, sp\n"
        //"mov r1, lr\n"
        //"mrs r2, spsr\n"
        //"mov r3, fp\n"
        "cps %0\n"
        //"msr spsr, r2\n"
        "isb\n"
        "mov sp, r0\n"
        //"mov lr, r1\n"
        //"mov fp, r3\n"
        "dsb"
        :
        : "I" (SVC_MODE)
        : "r0", "r1", "r2", "r3", "sp", "cc", "memory"
    );
    init_print();
    printf("Done!\n");
    print_magic();
    printf("Local magic: 0x%08X\n", local_magic);

    printf("Press any button to read SCTLR\n");
    (void) getc();
    u_int32_t sctlr_ns_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "str r0, %0"
        : "=m" (sctlr_ns_val)
        :
        : "r0"
    );
    printf("SCTLR: 0x08%" PRIx32 "\n", sctlr_ns_val);

    printf("Press any button to fix SCTLR\n");
    printf("Trying to put 0x%08" PRIx32 " from @0x%08" PRIx32 "!\n", sctlr_val, (u_int32_t)&sctlr_val);
    (void) getc();
    asm volatile(
        "ldr r0, %0\n"
        "mcr p15, 0, r0, c1, c0, 0\n"
        "dsb\n"
        "isb\n"
        :
        : "m" (sctlr_val)
        : "r0"
    );
    printf("Done!\n");

    printf("Press any button to read SCTLR\n");
    sctlr_ns_val = 241;
    (void) getc();
        asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "str r0, %0"
        : "=m" (sctlr_ns_val)
        :
        : "r0"
    );
    printf("SCTLR: 0x08%" PRIx32 "\n", sctlr_ns_val);

    printf("Press any button to read TTBCR\n");
    (void)getc();
    asm volatile(
        "mrc p15, 0, r0, c2, c0, 2\n"
        "str r0, %0"
        : "=m" (ttbcr_val)
        :
        : "r0"
    );
    printf("TTBCR: 0x%08" PRIx32 "\n", ttbcr_val);

    printf("Press any button to read TTBR0\n");
    (void)getc();
    asm volatile(
        "mrrc p15, 0, r0, r1, c2\n"
        "str r0, %0\n"
        "str r1, %1\n"
        : "=m" (ttbr0_low_val), "=m" (ttbr0_high_val)
        :
        : "r0", "r1"
    );
    printf("TTBR0: low - 0x%08" PRIx32 ", high - 0x%08" PRIx32 "\n", ttbr0_low_val, ttbr0_high_val);

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

    printf("Press \'s\' to exit, any other button to continue\n");
    if(getc() == 's')
        return 241;

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
    printf("CPSR: 0x%08" PRIx32 "\n", cpsr_val);

    printf("Press any button to read HSCTLR\n");
    (void) getc();
    uint32_t hsctlr_val;
    asm volatile(
        "mrc p15, 0, r0, c1, c0, 0\n"
        "str r0, %0"
        : "=m" (hsctlr_val)
        :
        : "r0"
    );
    printf("HSCTLR: 0x08%" PRIx32 "\n", hsctlr_val);

    printf("Press any button to read HTTBR\n");
    (void)getc();
    u_int32_t httbr_low_val, httbr_high_val;
    asm volatile(
        "mrrc p15, 4, r0, r1, c2\n"
        "str r0, %0\n"
        "str r1, %1\n"
        : "=m" (httbr_low_val), "=m" (httbr_high_val)
        :
        : "r0", "r1"
    );
    printf("HTTBR: low - 0x%08" PRIx32 ", high - 0x%08" PRIx32 "\n", httbr_low_val, httbr_high_val);

    (void) getc();

    return 0;
}

