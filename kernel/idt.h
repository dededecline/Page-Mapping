#ifndef _IDT_H_
#define _IDT_H_

#include "stdint.h"
#include "mmu.h"

#define IDT_COUNT 256

extern Descriptor idt[];

extern TableDescriptor idtDesc;

extern void initIdt(void);

#endif
