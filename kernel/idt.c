#include "idt.h"
#include "gdt.h"
#include "machine.h"

Descriptor idt[IDT_COUNT];

TableDescriptor idtDesc = {
    .limit = (uint16_t) (sizeof(idt) - 1),
    .base = (uint32_t) idt
};

void initIdt(void) {
    setTrapDescriptor(&idt[14],kernelCS,(uint32_t)pageFaultHandler,0);
}
