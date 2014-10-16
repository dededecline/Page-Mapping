#include "tss.h"
#include "machine.h"
#include "gdt.h"

//#define STACK_LONGS 1024

//uint32_t stack0[STACK_LONGS];

struct {
    uint32_t prev;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t unused[19];
} tss;

void initTss(void) {
    //tss.esp0 = (uint32_t)&stack0[STACK_LONGS-1];
    tss.ss0 = globalSelector(GDT_KERNEL_DATA,0);
    setTssDescriptor(&gdt[GDT_TSS],(uint32_t)&tss,sizeof(tss));
    ltr(globalSelector(GDT_TSS,0));
}

void setEsp0(uint32_t esp0) {
    tss.esp0 = esp0;
}
