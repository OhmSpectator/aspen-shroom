.global hyp_vectors
.align 5
hyp_vectors:
    b .                  @ 0x00 Not used
    b .                  @ 0x04 Undefined Instruction
    ldr pc, _hyp_handler @ 0x08 Hypervisor Call
    b .                  @ 0x0C Prefetch Abort
    b .                  @ 0x10 Data Abort
    b .                  @ 0x14 Hyp Trap, or Hyp mode entry
    b .                  @ 0x18 IRQ Interrupt
    b .                  @ 0x1C FIQ Interrupt

.global _hyp_handler
_hyp_handler:
    mov pc, #0

