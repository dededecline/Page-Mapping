#ifndef _GDT_H_
#define _GDT_H_

#ifndef ASSEMBLY
#include "stdint.h"
#include "mmu.h"
#endif

#define GDT_NULL                0
#define GDT_KERNEL_CODE         1
#define GDT_KERNEL_DATA         2
#define GDT_TSS                 3
#define GDT_USER_CODE           4
#define GDT_USER_DATA           5

#define GDT_COUNT               10

#ifndef ASSEMBLY
extern Descriptor gdt[];

extern uint32_t kernelCS;
extern uint32_t kernelDS;

extern TableDescriptor gdtDesc;

extern void initGdt(void);
#endif

#endif
