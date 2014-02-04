/* Exceptions - assembly exception handling. */

//Layout should be:
//     -reset
//     -undefined instruction
//     -software interrupt/supervisor call
//     -prefetch abort
//     -data abort
//     -Not used (used for hypervisor)
//     -IRQ interrupt
//     -FIQ interrupt

.globl _exceptions_vector
_exceptions_vector:
     b Start //Reset
     ldr    pc, [pc, #24] //Undefined Inst.
     ldr    pc, [pc, #24] //Software Interrupt
     ldr    pc, [pc, #24] //Prefetch abort
     ldr    pc, [pc, #24] //Data Abort
     ldr    pc, [pc, #24] //Reserved
     // IRQ, FIQ
     ldr    pc, [pc, #24] //Reset
     ldr    pc, [pc, #24] //Reset

.word   Start
.word   undef
.word   swi
.word   prefetch_abort
.word   data_abort
.word   reserved
.word   IRQ
.word   FIQ

.weak   reset
.set    reset, __reset
.weak   undef
.set    undef, __undefined_instruction 
.weak   swi
.set    swi, __software_interrupt
.weak   prefetch_abort
.set    prefetch_abort, __prefetch_abort
.weak   data_abort
.set    data_abort, __data_abort
.weak   reserved
.set    reserved, 0
.weak   IRQ
.set    IRQ, __irq_exception  
.weak   FIQ
.set    FIQ, __fiq_exception
