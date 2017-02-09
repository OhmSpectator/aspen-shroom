.global hyp_vectors
.align 5
hyp_vectors:
    .word 0              @ 0x00 Not used
    .word 0              @ 0x04 Undefined Instruction
    adr pc, _hyp_handler @ 0x08 Hypervisor Call
    .word 0              @ 0x0C Prefetch Abort
    .word 0              @ 0x10 Data Abort
    .word 0              @ 0x14 Hyp Trap, or Hyp mode entry
    .word 0              @ 0x18 IRQ Interrupt
    .word 0              @ 0x1C FIQ Interrupt

.global _hyp_handler
_hyp_handler:
    mov pc, #0

